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
    std::cout << games2.size() << std::endl; // (matching tag count + matching tag count) / (total number of the tags between the two games)
    // use a frequency map to count the total number of tag occurences to determine "popular" tags i.e. >100 (number may change)
    // typically, if a game has 1 tag, it usually has atleast 2
    // if a game has small amount of tags, select the least popular one to do the connections, if that least popular one is still 'popular', then require the next least popular tag also
    // TODO: fix frequency map (it adds the from tags as a repeat)
    // TODO: set up the program to insert a node, then from that node insert similar games to itself, limit the games by like the top x most weighted nodes
    // idea; the number of tags a game has defines how many neighbors it has
    parser.parse_tag_count();
    parser.print_tag_freq();
    /*
        if keys with matching tags size exceeds 100
            take the next tag of the source and add one to the requirements


     */

    return 0;
}