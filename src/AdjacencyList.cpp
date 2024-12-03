//
// Created by eboyh on 11/22/2024.
//

#include "../include/AdjacencyList.h"
#include <iomanip>
#include <queue>
#include <limits>
#include <vector>
#include <algorithm>
#include <utility>
void AdjList::insert(const std::string& from, const std::string& to, const std::vector<std::string>& from_tags, const std::vector<std::string>& to_tags) {
    for (auto& edge : adjlist[from]){ // skip already inserted games
        if (edge.first == to){
            return;
        }
    }
    if (adjlist.find(from) == adjlist.end()) {
        adjlist[from] = std::vector<std::pair<std::string, float>>();
        tags[from].insert(from_tags.begin(), from_tags.end());
    }

    if (adjlist.find(to) == adjlist.end()) {
        adjlist[to] = std::vector<std::pair<std::string, float>>();
        tags[to].insert(to_tags.begin(), to_tags.end());
    }

    adjlist[from].emplace_back(to, 0);
    adjlist[to].emplace_back(from, 0);
    calculate_weights(from, to, from_tags, to_tags);
}

void AdjList::print() {
    for (auto& item : adjlist){
        std::cout << item.first << ": ";
        for (auto& tag : item.second){
            std::cout << "(" << tag.first << ", with weight:  " << tag.second << ") ";
        }
        std::cout << std::endl;
    }
}

void AdjList::calculate_weights(const std::string& from, const std::string& to, const std::vector<std::string>& from_tags, const std::vector<std::string>& to_tags) {
    std::vector<std::string> vec1 = from_tags;
    std::vector<std::string> vec2 = to_tags;

    std::sort(vec1.begin(), vec1.end());
    std::sort(vec2.begin(), vec2.end());

    std::vector<std::string> intersection;
    std::set_intersection(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(),
                          std::back_inserter(intersection));
    int count = intersection.size();

    float weight = 1.0f - ((float)(count * 2) / (float)(vec1.size() + vec2.size()));
    adjlist[from].back().second = weight;

    for (auto& edge : adjlist[to]) { // find the corresponding edge for 'to' to update its weight
        if (edge.first == from) {
            edge.second = weight;
            break;
        }
    }
}

void AdjList::initialize_graph(std::string game, std::vector<std::string> tags,
                               std::unordered_map<std::string, std::vector<std::string>>& games,
                               int match_requirement) {
    if (visited.find(game) != visited.end()) {
        return; // skip already visited games
    }

    if (match_requirement == 0 || size >= 100){
        return;
    }

    // initialize match_requirement for the first call for each game
    if (match_requirement == -1) {
        match_requirement = tags.size();
    }
    bool inserted = false;

    for (auto& entry : games) {
        const std::string& other_game = entry.first;
        const std::vector<std::string>& other_tags = entry.second;

        // skip the current game
        if (other_game == game) {
            continue;
        }

        bool edge_exists = false;
        for (auto& edge : adjlist[game]) {
            if (edge.first == other_game) {
                edge_exists = true; // check if the edge already exists between games
                break;
            }
        }

        if (edge_exists) {
            continue; // skip if the edge already exists
        }

        // count matching tags
        int match_count = 0;
        for (const auto& tag : tags) {
            if (std::find(other_tags.begin(), other_tags.end(), tag) != other_tags.end()) {
                match_count++;
            }
        }

        // add the game if it satisfies the requirement
        if (match_count >= match_requirement) {
            insert(game, other_game, tags, other_tags);
            size++;
            inserted = true;
            initialize_graph(other_game, other_tags, games, -1);
            if (size >= 100) {
                return;
            }
        }
    }
    if (!inserted) {
        if (match_requirement > 1) {
            initialize_graph(game, tags, games, match_requirement - 1);
        } else {
            visited.insert(game);
        }
    }
}

int AdjList::get_size() {
    return size;
}

std::vector<std::string> AdjList::Dijsktras(const std::string& start, const std::string& destination) {
    std::unordered_map<std::string, float> distance;
    std::unordered_set<std::string> visited;
    std::unordered_map<std::string, std::string> previous;
    using Pair = std::pair<float, std::string>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<>> pq;

    for (const auto& pair : adjlist) {
        distance[pair.first] = std::numeric_limits<float>::infinity();
    }
    distance[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        Pair top = pq.top();
        pq.pop();
        float current_distance = top.first;
        std::string current_node = top.second;

        if (visited.count(current_node)) {
            continue;
        }
        visited.insert(current_node);

        for (const auto& neighbor : adjlist[current_node]) {
            const std::string& neighbor_node = neighbor.first;
            float weight = neighbor.second;

            if (current_distance + weight < distance[neighbor_node]) {
                distance[neighbor_node] = current_distance + weight;
                previous[neighbor_node] = current_node;
                pq.push({distance[neighbor_node], neighbor_node});
            }
        }
    }

    std::vector<std::string> path;
    for (std::string at = destination; at != ""; at = previous[at]) {
        path.push_back(at);
        if (at == start) break;
    }
    reverse(path.begin(), path.end());

    if (path.front() != start) return {};

    return path;
}


void AdjList::addEdge(const std::string& from, const std::string& to, float weight) {
    this->adjlist[from].emplace_back(to, weight);
    this->adjlist[to].emplace_back(from, weight); // add reverse edge
}

void AdjList::graphToPNG(const std::vector<std::string>& path) { // using https://graphviz.org/documentation/
    std::string dotString = "strict graph { \n"; // start dot file string

    for(const auto& [from, adjacent] : this->adjlist){ // add all edges to dot file
        for(const auto& [to, weight] : adjacent){
            dotString.append( "  ");
            dotString.append(from);
            dotString.append(" -- ");
            dotString.append(to);
            dotString.append(" [label=");
            dotString.append(std::to_string(weight));
            dotString.append(" color=\"black\"]");
        }
    }

    // color the path of shortest vertex

    dotString.append("}");

}

std::vector<std::string> AdjList::BellmanFord(const std::string& source, const std::string& end) { // using slides 29-41 from module 11's discussion google doc
    if(source == end){
        return {};
    }

    // initialize distance and predecessor maps
    std::unordered_map<std::string, float> distance_map;
    std::unordered_map<std::string, std::string> predecessor;

    // set all distance to infinity
    for(const auto& pair : this->adjlist){
        // using https://en.cppreference.com/w/cpp/types/numeric_limits/infinity
        distance_map[pair.first] = std::numeric_limits<float>::infinity();
    }
    distance_map[source] = 0; // set source to 0

    // relax each edge V-1 times
    for(auto i = 0; i < this->adjlist.size() - 1; i++){
        // using https://en.cppreference.com/w/cpp/language/structured_binding
        for(const auto& [from, neighbors] : this->adjlist){
            for(const auto& [to, weight] : neighbors){
                if(distance_map[from] + weight < distance_map[to]){
                    distance_map[to] = distance_map[from] + weight;
                    predecessor[to] = from;
                }
            }
        }
    }

    // attempt to relax all edges one more time and if a distance changes then there is a negative weight cycle
    for(const auto& [from, neighbors] : this->adjlist){
        for(const auto& [to, weight] : neighbors){
            if(distance_map[from] + weight < distance_map[to]){
                std::cout << "Error: Graph contains a negative-weight cycle (From Bellman-Ford method)" << std::endl;
                return {};
            }
        }
    }

    // if there is no path to end then return empty vector
    if(predecessor.find(end) == predecessor.end()){
        return {};
    }

    std::vector<std::string> source_to_end_path;
    std::string current = end; // add end game
    // add each predecessor that exists until reaching the source node
    while(predecessor.find(current) != predecessor.end()){
        source_to_end_path.push_back(current);
        current = predecessor.at(current);
    }
    source_to_end_path.push_back(current);

    // source_to_end_path.push_back(current); // add source game
    std::reverse(source_to_end_path.begin(), source_to_end_path.end()); // reverse list

    return source_to_end_path;
}






void testGraphAlgorithms(){ // put this at the bottom it's just for testing, but it's a lot of lines of code
    // test 1
    AdjList graph1;
    graph1.addEdge("a", "b", 1);
    graph1.addEdge("b", "c", 1);
    graph1.addEdge("c", "d", 1);

    std::cout << "Test 1: simple path (should be \"a b c d\"): ";
    std::vector<std::string> testPath = graph1.BellmanFord("a", "d");

    if(testPath.empty()){
        std::cout << "no path";
    }
    for(const std::string& game : testPath){
        std::cout << game << " ";
    }
    std::cout << std::endl;


    // test 2
    AdjList graph2;
    graph2.addEdge("a", "b", 1);
    graph2.addEdge("b", "c", 1);
    graph2.addEdge("c", "d", -100);
    graph2.addEdge("b", "d", 1);

    std::cout << "Test 2: negative cycle (should be only error message): ";
    testPath = graph2.BellmanFord("a", "d");

    if(testPath.empty()){
        std::cout << "no path";
    }
    for(const std::string& game : testPath){
        std::cout << game << " ";
    }
    std::cout << std::endl;


    // test 3
    AdjList graph3;
    graph3.addEdge("a", "b", 1);
    graph3.addEdge("c", "d", 1);

    std::cout << "Test 3: disconnected graph (should be \"no path\"): ";
    testPath = graph3.BellmanFord("a", "d");

    if(testPath.empty()){
        std::cout << "no path";
    }
    for(const std::string& game : testPath){
        std::cout << game << " ";
    }
    std::cout << std::endl;


    // test 4
    AdjList graph4;
    graph4.addEdge("a", "b", 100);
    graph4.addEdge("b", "c", 1);
    graph4.addEdge("c", "d", 1);
    graph4.addEdge("a", "i", 1);
    graph4.addEdge("i", "j", 1);
    graph4.addEdge("j", "d", 1);


    std::cout << "Test 4: multiple paths (should be \"a i j d\"): ";
    testPath = graph4.BellmanFord("a", "d");

    if(testPath.empty()){
        std::cout << "no path";
    }
    for(const std::string& game : testPath){
        std::cout << game << " ";
    }
    std::cout << std::endl;


    // test 5
    AdjList graph5;
    graph5.addEdge("a", "a", 1);

    std::cout << "Test 5: one vertex (should be \"no path\"): ";
    testPath = graph5.BellmanFord("a", "d");

    if(testPath.empty()){
        std::cout << "no path";
    }
    for(const std::string& game : testPath){
        std::cout << game << " ";
    }
    std::cout << std::endl;


    // test 6
    AdjList graph6;
    graph6.addEdge("a", "b", 1);
    graph6.addEdge("c", "d", 1);

    std::cout << "Test 6: no path exists (should be \"no path\"): ";
    testPath = graph6.BellmanFord("a", "d");

    if(testPath.empty()){
        std::cout << "no path";
    }
    for(const std::string& game : testPath){
        std::cout << game << " ";
    }
    std::cout << std::endl;


    // test 7
    AdjList graph7;
    for(int i = 1; i < 101; i++){
        graph7.addEdge(std::to_string(i), std::to_string(i+1), 1);
    }

    std::cout << "Test 7: very large graph (should be \"1 - 100\"): ";
    testPath = graph7.BellmanFord("0", "100");

    if(testPath.empty()){
        std::cout << "no path";
    }
    for(const std::string& game : testPath){
        std::cout << game << " ";
    }
    std::cout << std::endl;


    // test 8
    AdjList graph8;
    graph8.addEdge("a", "b", 0);
    graph8.addEdge("b", "c", 0);
    graph8.addEdge("c", "d", 1);

    std::cout << "Test 8: 0 weight edges (should be \"a b c d\"): ";
    testPath = graph8.BellmanFord("a", "d");

    if(testPath.empty()){
        std::cout << "no path";
    }
    for(const std::string& game : testPath){
        std::cout << game << " ";
    }
    std::cout << std::endl;


    // test 9
    AdjList graph9;
    graph9.addEdge("a", "b", -5);
    graph9.addEdge("b", "a", 10);
    graph9.addEdge("c", "d", 100);

    std::cout << "Test 9: negative edge weights but no negative cycles (should be error message still): ";
    testPath = graph9.BellmanFord("a", "d");

    if(testPath.empty()){
        std::cout << "no path";
    }
    for(const std::string& game : testPath){
        std::cout << game << " ";
    }
    std::cout << std::endl;
}


