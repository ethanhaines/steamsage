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

    std::unordered_map<std::string, std::vector<std::string>> games = parser.parse(); // this contains every game in our csv
    std::cout << games.size() << std::endl; // (matching tag count + matching tag count) / (total number of the tags between the two games)
    std::unordered_map<std::string, int> tag_frequency = parser.parse_tag_count();
    std::cout << "Enter a game you like: (for now this has to be exact match but later we will use a drop down search list): " << std::endl;
    std::string game;
    std::getline(std::cin, game);
    std::cout << game << std::endl;
    std::vector<std::string> input_tags = games[game];
    graph.initialize_graph(game, input_tags, games, tag_frequency, 0, -1);
    graph.print();
    //probably want to start using modularity and creating functions for everything, like init() for parsing

    return 0;
}
