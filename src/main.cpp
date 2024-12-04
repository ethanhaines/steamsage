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

// I wanted to find a way for the user to be able to see if they made a typo or if the game they wanted didn't exist in our csv
// So i found something called levenshtein distance for finding string similarity, and i found a stack overflow and
// used the logic that was originally in java to create these functions
// I also just used other stack overflows to help me understand levenshtein distance further
// citations:
// https://stackoverflow.com/questions/36472793/levenshtein-distance-algorithm
// https://stackoverflow.com/questions/31625387/levenshtein-distance-where-i-only-care-about-words
// https://stackoverflow.com/questions/3183149/most-efficient-way-to-calculate-levenshtein-distance
int levenshtein_distance(const std::string& s1, const std::string& s2) {
    int m = s1.size();
    int n = s2.size();

    if (m < n) return levenshtein_distance(s2, s1); // s1 has to be longer

    std::vector<int> prev_row(n + 1);
    std::vector<int> curr_row(n + 1); // memory efficiency in storing just the rows and using them, instead of a whole matrix

    for (int j = 0; j <= n; j++) {
        prev_row[j] = j;
    }

    for (int i = 1; i <= m; i++) {
        curr_row[0] = i;
        for (int j = 1; j <= n; j++) {
            int cost;
            if (s1[i - 1] == s2[j - 1]) {
                cost = 0;
            } else {
                cost = 1;
            }
            curr_row[j] = std::min({prev_row[j] + 1,curr_row[j - 1] + 1,prev_row[j - 1] + cost});
        }
        std::swap(prev_row, curr_row);
    }

    return curr_row[n];
}


std::string find_most_similar(const std::vector<std::string>& keys, const std::string& input) { // takes in all keys and compares them to user input
    int min_distance = std::numeric_limits<int>::max();
    std::string most_similar;

    for (const auto& str : keys) {
        int distance = levenshtein_distance(input, str);
        if (distance < min_distance) {
            min_distance = distance;
            most_similar = str;
        }
    }

    return most_similar;
}

std::string get_game(std::string possible_game,  std::vector<std::string>& keys, std::unordered_map<std::string, std::vector<std::string>>& games){
    while(true){
        if (games.find(possible_game) == games.end()) {
            possible_game = find_most_similar(keys, possible_game);
            std::cout << "Did you mean: " << possible_game << std::endl;
            std::cout << "Enter Y or y if this is the game you want, enter anything else to re-input game." << std::endl;

            std::string user_input;
            std::getline(std::cin, user_input);

            if (user_input == "Y" || user_input == "y") {
                return possible_game;
            }else{
                std::cout << "Re-enter game: " << std::endl;
                std::getline(std::cin, possible_game);
            }
        }else{
            return possible_game;
        }
    }
}

void input(std::unordered_map<std::string, std::vector<std::string>>& games, std::vector<std::string>& keys, AdjList& graph){
    // TODO: somehow loop
    std::cout << "Enter a game you like: " << std::endl;
    std::string possible_game;
    std::getline(std::cin, possible_game);

    std::vector<std::string> search_path;
    std::string game = get_game(possible_game, keys, games);

    // std::cout << "try to initialize graph" << std::endl;
    graph.initialize_graph(game, games[game], games, -1);
    graph.graphToPNG({},false);
    // TODO: graph should be displayed here
    std::cout << "Choose a game from the graph that you would like to find similar games for: " << std::endl;
    std::string possible_game2;
    std::getline(std::cin, possible_game2);
    std::string game2 = get_game(possible_game2, keys, games);
    std::cout << "Input 1 to search for this game using Dijsktras, anything else for Bellman-Ford" << std::endl;
    int in;
    std::cin >> in;
    if (in == 1){
        search_path = graph.Dijsktras(game, game2);
    }
    else{
        search_path = graph.BellmanFord(game, game2);
    }
    graph.graphToPNG(search_path, true);

    // TODO: display the time of the search path and/or both/either/one
}

void init(Parser& p, AdjList& g, std::unordered_map<std::string, std::vector<std::string>>& games, std::vector<std::string>& keys){
    games = p.parse();

    for (const auto& i : games){
        keys.push_back(i.first);
    }

    input(games, keys, g);
}

int main() {

    Parser parser("../resources/games.csv");
    AdjList graph;
    std::unordered_map<std::string, std::vector<std::string>> games;
    std::vector<std::string> keys;
    init(parser, graph, games, keys);
    //input game
    //display graph
    //select game for similarity
    //select algorithm
    //display pathed graph
    //possibly repeat so the user can select new algo
    //graph.initialize_graph(game, games[game], games, -1);
    //graph.graphToPNG(graph.BellmanFord("Conviction", "DOOM Eternal"), true);
    //std::cout << "bellman took " << graph.get_bellman_time() << " seconds" << std::endl;
    return 0;
}
