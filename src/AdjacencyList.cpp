//
// Created by eboyh on 11/22/2024.
//

#include "../include/AdjacencyList.h"
#include <iomanip>
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

std::unordered_map<std::string, int> AdjList::get_tag_count() {
    return tag_count;
}

void AdjList::initialize_graph(std::string game, std::vector<std::string> tags,
                               std::unordered_map<std::string, std::vector<std::string>>& games,
                               int match_requirement) {
    if (visited.find(game) != visited.end()) {
        return; // skip already visited games
    }


    if (match_requirement == 0){
        return;
    }
    // Base case to stop recursion
    if (size >= 100) {
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





