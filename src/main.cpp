//
// Created by eboyh on 11/22/2024.
//

#include <iostream>
#include <../include/Parser.h>
#include <unordered_map>
#include <string>
#include <vector>
int main() {

    Parser parser("../resources/games.csv");

    std::unordered_map<std::string, std::vector<std::string>> games2 = parser.parse();
    std::cout << games2.size() << std::endl;

    return 0;
}