//
// Created by eboyh on 11/22/2024.
//

#include "../include/AdjacencyList.h"
#include <iomanip>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <utility>
using namespace std;
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

vector<string> AdjList::Dijsktras(const string& start, const string& destination) {
    unordered_map<string, float> distance;
    unordered_set<string> visited;
    unordered_map<string, string> previous;
    using Pair = pair<float, string>;
    priority_queue<Pair, vector<Pair>, greater<>> pq;

    for (const auto& pair : adjlist) {
        distance[pair.first] = numeric_limits<float>::infinity();
    }
    distance[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        Pair top = pq.top();
        pq.pop();
        float current_distance = top.first;
        string current_node = top.second;

        if (visited.count(current_node)) {
            continue;
        }
        visited.insert(current_node);

        for (const auto& neighbor : adjlist[current_node]) {
            const string& neighbor_node = neighbor.first;
            float weight = neighbor.second;

            if (current_distance + weight < distance[neighbor_node]) {
                distance[neighbor_node] = current_distance + weight;
                previous[neighbor_node] = current_node;
                pq.push({distance[neighbor_node], neighbor_node});
            }
        }
    }

    vector<string> path;
    for (string at = destination; at != ""; at = previous[at]) {
        path.push_back(at);
        if (at == start) break;
    }
    reverse(path.begin(), path.end());

    if (path.front() != start) return {};

    return path;
}


void AdjList::print_tag_frequencies() {
    std::cout << "Tag Frequencies:" << std::endl;
    for (const auto& entry : tag_count) {
        std::cout << "Tag: " << entry.first << " Frequency: " << entry.second << std::endl;
    }
}

// TODO: Implement sorting algorithms



