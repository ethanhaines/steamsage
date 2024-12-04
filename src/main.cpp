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
    std::cout << "Enter a game you like: (for now this has to be exact match but later we will use a drop down search list): " << std::endl;
    std::string game;
    std::getline(std::cin, game);
    graph.initialize_graph(game, games[game], games, -1);


    return 0;
}
