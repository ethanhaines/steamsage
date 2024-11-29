//
// Created by eboyh on 11/22/2024.
//

#include <iostream>
#include <../include/Parser.h>
#include "../include/AdjacencyList.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

int main() {

    Parser parser("../resources/games.csv");
    AdjList graph;

    std::unordered_map<std::string, std::vector<std::string>> games2 = parser.parse(); // this contains every game in our csv
    std::cout << games2.size() << std::endl; // (matching tag count * 2) / (total number of the tags between the two games)
    std::unordered_map<std::string, int> tag_count;
    int count = 0;
    int indie_count = 0;
    for (auto it = games2.begin(); count < games2.size(); it++) {
        count++;
        if (std::find(it->second.begin(), it->second.end(), "Bowling") != it->second.end()) {
            indie_count++;
            graph.insert("Galactic Bowling", it->first, games2["Galactic Bowling"], games2[it->first]);
        }
    }
    /*
        if keys with matching tags size exceeds 100
            take the next tag of the source and add one to the requirements


     */

    graph.print();
    std::cout << indie_count << std::endl;
    return 0;
}