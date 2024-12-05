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
#include <SFML/Graphics.hpp>  // Include SFML Graphics
#include <SFML/Window.hpp>

/*
 * SFML sources used:
 * git repository    https://github.com/SFML/SFML
 * example code      https://github.com/SFML/cmake-sfml-project/blob/master/CMakeLists.txt
 * cmake tutorial    https://www.youtube.com/watch?v=R3x2OvsUHOU
 * */


// I wanted to find a way for the user to be able to see if they made a typo or if the game they wanted didn't exist in our csv
// So i found something called levenshtein distance for finding string similarity, and i found a stack overflow and
// used the logic that was originally in java to create these functions
// I also just used other stack overflows to help me understand levenshtein distance further
// citations:
// https://stackoverflow.com/questions/36472793/levenshtein-distance-algorithm
// https://stackoverflow.com/questions/31625387/levenshtein-distance-where-i-only-care-about-words
// https://stackoverflow.com/questions/3183149/most-efficient-way-to-calculate-levenshtein-distance
// I also used the 12/4 discussion slides logic to help
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
        if (games.find(possible_game) == games.end()) { // loop until you find the game you want then return
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

void displayPNG(std::string filename){ // using https://stackoverflow.com/questions/24358968/loading-a-texture-in-sfml

    sf::Texture pngTexture;
    if(!pngTexture.loadFromFile(filename + ".png")){
        std::cerr << "Error: Loading SFML PNG" << std::endl;
        return;
    }

    sf::Vector2u pngSize = pngTexture.getSize();

    sf::Sprite pngSprite;
    pngSprite.setTexture(pngTexture);


    sf::RenderWindow window(sf::VideoMode(1000, 800), "Display Graph");
    window.requestFocus();

    sf::View view(sf::FloatRect(0.f, 0.f, 1000.f, 1000.f));
    window.setView(view);

    bool panning = false;
    sf::Vector2i start_pos;

    while (window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                panning = true;
                start_pos = sf::Mouse::getPosition(window);
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                panning = false;
            }

            if (event.type == sf::Event::MouseMoved && panning) {
                sf::Vector2i current_pos = sf::Mouse::getPosition(window);
                sf::Vector2f delta(
                        static_cast<float>(start_pos.x - current_pos.x),
                        static_cast<float>(start_pos.y - current_pos.y)
                );
                view.move(delta);
                window.setView(view); // apply the updated view
                start_pos = current_pos; // update the start position
            }

            window.clear(sf::Color(255, 255, 255));
            window.draw(pngSprite);
            window.display();
        }
    }
}

void input(std::unordered_map<std::string, std::vector<std::string>>& games, std::vector<std::string>& keys, AdjList& graph){
    std::cout << "Enter a game you like: " << std::endl;
    std::string possible_game;
    std::getline(std::cin, possible_game);
    std::string game = get_game(possible_game, keys, games);

    std::cout << "Loading graph (takes ~30 seconds)..." << std::endl;
    graph.initialize_graph(game, games[game], games, -1);
    std::cout << "Displaying graph..." << std::endl;
    graph.graphToPNG({},false, "graph_not_highlighted");

    displayPNG("graph_not_highlighted");

    bool bellman = false;

    std::vector<std::string> search_path;
    std::cout << "Choose a game from the graph that you also like, the most similar games between your two inputs will be along the shortest path: " << std::endl;

    std::string possible_game2;
    std::getline(std::cin, possible_game2);
    std::string game2 = get_game(possible_game2, keys, games);

    std::cout << "Input 1 to search for this game using Dijsktras, anything else for Bellman-Ford" << std::endl;
    int in;
    std::cin >> in;

    if (in == 1){
        search_path = graph.Dijsktras(game, game2);
        std::cout << "Dijsktras took: " << graph.get_dijsktras_time() << " seconds" << std::endl;
        bellman = false;
    }

    else{
        search_path = graph.BellmanFord(game, game2);
        std::cout << "Bellman-Ford took: " << graph.get_bellman_time() << " seconds" << std::endl;
        bellman = true;
    }

    std::cout << "Displaying chosen algorithm's search path..." << std::endl;
    graph.graphToPNG(search_path, true, "graph_highlighted");

    std::string in2;
    std::cout << "Would you like to attempt the search with the other algorithm? Enter Y or y: " << std::endl;
    std::cin >> in2;

    if (in2 == "Y" || in2 == "y"){
        if (!bellman){
            search_path = graph.BellmanFord(game, game2);
            std::cout << "Bellman-Ford took: " << graph.get_bellman_time() << " seconds" << std::endl;
        }

        else{
            search_path = graph.Dijsktras(game, game2);
            std::cout << "Dijsktras took: " << graph.get_dijsktras_time() << " seconds" << std::endl;
        }
    }

    std::cout << "Displaying other algorithm's search path..." << std::endl;
    graph.graphToPNG(search_path, true, "graph_highlighted");
    std::cout << "Thank you for using our program!" << std::endl;
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
    return 0;
}
