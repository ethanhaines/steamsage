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

    std::unordered_map<std::string, std::vector<std::string>> games2 = parser.parse();
    std::cout << games2.size() << std::endl;
    int count = 0;
    int indie_count = 0;
    for (auto it = games2.begin(); count < games2.size(); it++) {
        count++;
        if (std::find(it->second.begin(), it->second.end(), "Bowling") != it->second.end()) {
            indie_count++;
            graph.insert("Galactic Bowling", it->first, games2["Galactic Bowling"], games2[it->first]);
        }
    }
    graph.print();
    std::cout << indie_count << std::endl;
    return 0;
}