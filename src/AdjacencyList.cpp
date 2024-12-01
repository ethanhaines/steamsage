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
    std::vector<std::string> vec1 = from_tags;
    std::vector<std::string> vec2 = to_tags;
    std::sort(vec1.begin(), vec1.end());
    std::sort(vec2.begin(), vec2.end());
    int count = 0;
    float weight = 1.0f - ((float)(count * 2) / (float)(vec1.size() + vec2.size()));
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


// TODO: Implement sorting algorithms



