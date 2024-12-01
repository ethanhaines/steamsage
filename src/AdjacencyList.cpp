//
// Created by eboyh on 11/22/2024.
//

#include "../include/AdjacencyList.h"
#include <iomanip>
#include <vector>
#include <algorithm>
#include <utility>
void AdjList::insert(const std::string& from, const std::string& to, const std::vector<std::string>& from_tags, const std::vector<std::string>& to_tags) {
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
    std::vector vec1 = from_tags;
    std::vector vec2 = to_tags;
    std::sort(vec1.begin(), vec1.end());
    std::sort(vec2.begin(), vec2.end());
    int count = 0;
    for (auto& tag : vec1){
        std::cout <<"Adding tag: " << tag << " from: " << from << std::endl;
        tag_count[tag]++;
        if (std::find(vec2.begin(), vec2.end(), tag) != vec2.end()){
            count++;
        }
    }
    for (auto& tag : vec2) {
        std::cout <<"Adding tag: " << tag << " from: " << to << std::endl;
        if (std::find(vec1.begin(), vec1.end(), tag) == vec1.end()) {
            tag_count[tag]++;
        }
    }
    float weight = (float)(count * 2) / (float)(vec1.size() + vec2.size());
    adjlist[from].back().second = weight;
    for (auto& edge : adjlist[to]) { // find the corresponding edge for 'to' to update its weight
        if (edge.first == from) {
            edge.second = weight;
            break;
        }
    }
}

std::unordered_map<std::string, int> AdjList::get_tag_count() {
    return tag_count;
}

void AdjList::print_tag_frequencies() {
    std::cout << "Tag Frequencies:" << std::endl;
    for (const auto& entry : tag_count) {
        std::cout << "Tag: " << entry.first << " Frequency: " << entry.second << std::endl;
    }
}

std::vector<std::string> AdjList::BellmanFord(const std::string& source, const std::string& end) { // using slides 29-41 from module 11's discussion google doc
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
                std::cout << "Error: Graph contains a negative-weight cycle" << std::endl;
                return {};
            }
        }
    }

    // if there is no path to end then return empty vector
    if(predecessor.find(end) == predecessor.end()){
        return {};
    }

    // TODO: Check if we want to return the source and end games included. Currently includes both
    std::vector<std::string> source_to_end_path;
    std::string current = predecessor.at(end); // dont add end game
    // add each predecessor that exists until reaching the source node
    while(predecessor.find(current) != predecessor.end()){
        source_to_end_path.push_back(current);
        current = predecessor.at(current);
    }

    // source_to_end_path.push_back(current); // add source game
    std::reverse(source_to_end_path.begin(), source_to_end_path.end()); // reverse list

    return source_to_end_path;

    // code below is for returning a sorted vector of closest to furthest

    /*// using https://cplusplus.com/reference/vector/vector/vector/
    std::vector<std::pair<std::string, int>> distance_vector(distance_map.begin(), distance_map.end());
    // using https://en.cppreference.com/w/cpp/algorithm/sort
    // using https://en.cppreference.com/w/cpp/utility/functional/greater
    std::sort(distance_vector.begin(), distance_vector.end(), std::greater());

    std::vector<std::string> sorted_distances(distance_vector.size()); // jetBrains IDE was yelling at me to preallocate
    for(const auto& game : distance_vector){
        sorted_distances.push_back(game.first);
    }

    return sorted_distances;*/
}

void AdjList::Dijkstras() {

}


// TODO: Implement sorting algorithms



