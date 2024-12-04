//
// Created by eboyh on 11/22/2024.
//

#include "../include/AdjacencyList.h"
#include <iomanip>
#include <queue>
#include <limits>
#include <vector>
#include <algorithm>
#include <stack>
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


void AdjList::initialize_graph(std::string start_game, std::vector<std::string> start_tags,
                               std::unordered_map<std::string, std::vector<std::string>>& games,
                               int initial_match_requirement) {
    std::stack<std::tuple<std::string, std::vector<std::string>, int>> stack;

    // push the starting game onto the stack
    stack.emplace(start_game, start_tags, initial_match_requirement);

    while (!stack.empty()) {
        // get the top element from the stack in the form of a tuple
        auto [game, tags, match_requirement] = stack.top();
        stack.pop();

        // Skip already visited games
        if (visited.find(game) != visited.end()) {
            continue;
        }

        // Initialize match_requirement for the first call for each game
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

            // check if the edge already exists
            bool edge_exists = false;
            for (auto& edge : adjlist[game]) {
                if (edge.first == other_game) {
                    edge_exists = true;
                    break;
                }
            }

            if (edge_exists) {
                continue;
            }

            int match_count = 0;
            for (const auto& tag : tags) {
                if (std::find(other_tags.begin(), other_tags.end(), tag) != other_tags.end()) {
                    match_count++;
                }
            }

            if (match_count >= match_requirement) {
                insert(game, other_game, tags, other_tags);
                size++;
                inserted = true;

                // push the newly inserted game onto the stack
                stack.emplace(other_game, other_tags, -1);

                // stop if the graph reaches the size limit
                if (size >= 100) {
                    return;
                }
            }
        }

        // if no insertion occurred, decrease the match requirement or mark the game as visited
        if (!inserted) {
            if (match_requirement > 1) {
                stack.emplace(game, tags, match_requirement - 1);
            } else {
                visited.insert(game);
            }
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

// this website can test return strings
// https://dreampuf.github.io/GraphvizOnline/?engine=dot#strict%20graph%20%7B%0A%20%20%20%20a%20--%20b%20%5Bcolor%3Dblack%20label%3D100%5D%0A%20%20%20%20b%20--%20c%20%5Bcolor%3Dblack%20label%3D3%5D%0A%20%20%20%20%0A%20%20%20%20a%20--%20b%20%5Bcolor%3Dred%5D%0A%20%20%0A%7D

// using https://graphviz.org/documentation/
std::string AdjList::graphToPNG(const std::vector<std::string>& path, bool highlight_shortest_path) { // using https://graphviz.org/documentation/
    // start dot file string
    // std::cout << "start trying" << std::endl;
    std::string dotString = "strict graph { \n"; // "strict" graph makes it so undirected edges cant be added twice

    for(const auto& [from, adjacent] : this->adjlist){ // add all edges to dot file string
        for(const auto& [to, weight] : adjacent){
            // using https://stackoverflow.com/questions/57882748/how-do-i-remove-trailing-zeros-when-printing-a-floating-point-number
            std::string weightString = std::to_string(weight);
            weightString = weightString.substr(0, weightString.find_last_not_of('0') + 1);

            dotString.append( "  \"");
            dotString.append(from);
            dotString.append("\" -- \"");
            dotString.append(to);
            dotString.append("\" [label=");
            dotString.append(weightString);
            dotString.append(" color=\"black\"]\n");
        }
    }
    // std::cout << "start highlighting" <<std::endl;
    if(highlight_shortest_path && !path.empty()){ // if we want the shortest path to be highlighted, color the path red
        for(int i = 0; i < path.size() - 1; i++){
            dotString.append("  \"");
            dotString.append(path[i]);
            dotString.append("\" -- \"");
            dotString.append(path[i + 1]);
            dotString.append("\" [color=red]\n");
        }
    }

    dotString.append("}\n"); // end dot file
    // std::cout << "finish dot file" << std::endl;

    // create and write dot file
    //using https://cplusplus.com/reference/fstream/fstream/?kw=fstream
    std::ofstream dotFile("graph.dot"); // for now just using graph.dot for simplicity
    if(dotFile.is_open()){
        dotFile << dotString;
        dotFile.close();
        std::cout << "File " << "graph.dot" << " created:";
        if(highlight_shortest_path)
            std::cout << " highlighted" << std::endl;
        else
            std::cout << " NOT highlighted" << std::endl;

    }else{
        std::cerr << "Error opening DOT file to write" << std::endl;
        return "";
    }

    // using https://stackoverflow.com/questions/1494492/graphviz-how-to-go-from-dot-to-a-graph
    // using https://en.cppreference.com/w/cpp/utility/program/system
    int result = system("dot -Tpng graph.dot -o graph.png");
    if(result == 0)
        std::cout << "graph.dot successfully created as graph.png" << std::endl;
    else
        std::cerr << "Error translating DOT to PNG" << std::endl;

    return dotString;
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
                std::cerr << "Error: Graph contains a negative-weight cycle (From Bellman-Ford method)" << std::endl;
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


