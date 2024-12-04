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


std::string find_most_similar(const std::vector<std::string>& strings, const std::string& input) { // takes in all keys and compares them to user input
    int min_distance = std::numeric_limits<int>::max();
    std::string most_similar;

    for (const auto& str : strings) {
        int distance = levenshtein_distance(input, str);
        if (distance < min_distance) {
            min_distance = distance;
            most_similar = str;
        }
    }

    return most_similar;
}

void input(std::unordered_map<std::string, std::vector<std::string>>& games, std::vector<std::string>& keys, AdjList& graph){
    std::cout << "Enter a game you like: " << std::endl;
    std::string game;
    std::getline(std::cin, game);

    if (games.find(game) == games.end()) {

        std::cout << "Did you mean: " << find_most_similar(keys, game) << std::endl;
        std::cout << "Enter Y if this is the game you want, enter anything else to re-input game." << std::endl;

        std::string user_input;
        std::cin >> user_input;

        if (user_input != "Y"){
            input(games, keys, graph);
            return;
        }
    }
    graph.initialize_graph(game, games[game], games, -1);
    //graph should be displayed here
    std::cout << "Choose a game from the graph that you would like to find similar games for: " << std::endl;
    std::string game2;
    std::getline(std::cin, game2);
    std::cout << "Input 1 to search for this game using Dijsktras, anything else for Bellman-Ford" << std::endl;
    int in;
    std::cin >> in;
    if (in == 1){
        std::vector<std::string> dijsktras_path = graph.Dijsktras(game, game2);
    }
    else{
        std::vector<std::string> bellman_path = graph.BellmanFord(game, game2);
    }
}

void init(Parser& p, AdjList& g, std::unordered_map<std::string, std::vector<std::string>>& games, std::vector<std::string>& keys){
    games = p.parse();

    for (auto i : games){
        keys.push_back(i.first);
    }

    input(games, keys, g);
}

int main() {

    Parser parser("../resources/games.csv");
    AdjList graph;
    std::unordered_map<std::string, std::vector<std::string>> games;
    std::vector<std::string> keys;
    std::string dot_data = "strict graph { \n"
                           "  \"QUAKE\" -- \"DOOM 64\" [label=0.25 color=\"black\"]\n"
                           "  \"QUAKE\" -- \"QUAKE II\" [label=0.1 color=\"black\"]\n"
                           "  \"DOOM\" -- \"DOOM 64\" [label=0.25 color=\"black\"]\n"
                           "  \"QUAKE II\" -- \"DOOM 64\" [label=0.25 color=\"black\"]\n"
                           "  \"QUAKE II\" -- \"QUAKE\" [label=0.1 color=\"black\"]\n"
                           "  \"Cathedral 3-D\" -- \"DOOM 64\" [label=0.25 color=\"black\"]\n"
                           "  \"Master Levels for Doom II\" -- \"Blood: One Unit Whole Blood\" [label=0.263158 color=\"black\"]\n"
                           "  \"Master Levels for Doom II\" -- \"Ultimate Doom\" [label=0.105263 color=\"black\"]\n"
                           "  \"Master Levels for Doom II\" -- \"Final DOOM\" [label=0.105263 color=\"black\"]\n"
                           "  \"Master Levels for Doom II\" -- \"DOOM II\" [label=0.157895 color=\"black\"]\n"
                           "  \"Master Levels for Doom II\" -- \"DOOM 64\" [label=0.210526 color=\"black\"]\n"
                           "  \"Blood: One Unit Whole Blood\" -- \"Blood: Fresh Supply\" [label=0.25 color=\"black\"]\n"
                           "  \"Blood: One Unit Whole Blood\" -- \"Master Levels for Doom II\" [label=0.263158 color=\"black\"]\n"
                           "  \"Blood: One Unit Whole Blood\" -- \"Final DOOM\" [label=0.3 color=\"black\"]\n"
                           "  \"DOOM 64\" -- \"Blood: Fresh Supply\" [label=0.25 color=\"black\"]\n"
                           "  \"DOOM 64\" -- \"Ultimate Doom\" [label=0.15 color=\"black\"]\n"
                           "  \"DOOM 64\" -- \"Final DOOM\" [label=0.15 color=\"black\"]\n"
                           "  \"DOOM 64\" -- \"DOOM II\" [label=0.2 color=\"black\"]\n"
                           "  \"DOOM 64\" -- \"Master Levels for Doom II\" [label=0.210526 color=\"black\"]\n"
                           "  \"DOOM 64\" -- \"Ion Fury\" [label=0.25 color=\"black\"]\n"
                           "  \"DOOM 64\" -- \"Cathedral 3-D\" [label=0.25 color=\"black\"]\n"
                           "  \"DOOM 64\" -- \"QUAKE II\" [label=0.25 color=\"black\"]\n"
                           "  \"DOOM 64\" -- \"DOOM\" [label=0.25 color=\"black\"]\n"
                           "  \"DOOM 64\" -- \"QUAKE\" [label=0.25 color=\"black\"]\n"
                           "  \"Operation Body Count\" -- \"Blood: Fresh Supply\" [label=0.210526 color=\"black\"]\n"
                           "  \"Sinister Halloween\" -- \"Invention 3\" [label=0.179487 color=\"black\"]\n"
                           "  \"Sinister Halloween\" -- \"Cold Cable: Lifeshift\" [label=0.25 color=\"black\"]\n"
                           "  \"Final DOOM\" -- \"Blood: One Unit Whole Blood\" [label=0.3 color=\"black\"]\n"
                           "  \"Final DOOM\" -- \"DOOM II\" [label=0.1 color=\"black\"]\n"
                           "  \"Final DOOM\" -- \"Ultimate Doom\" [label=0.15 color=\"black\"]\n"
                           "  \"Final DOOM\" -- \"Master Levels for Doom II\" [label=0.105263 color=\"black\"]\n"
                           "  \"Final DOOM\" -- \"DOOM 64\" [label=0.15 color=\"black\"]\n"
                           "  \"Serious Sam Classic: The First Encounter\" -- \"The Indie Game Legend 3D\" [label=0.35 color=\"black\"]\n"
                           "  \"Project RIP\" -- \"L.S.S II\" [label=0.3 color=\"black\"]\n"
                           "  \"Forkscythe\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"Chasm: The Rift\" -- \"KARM\" [label=0.25 color=\"black\"]\n"
                           "  \"Chasm: The Rift\" -- \"The Citadel\" [label=0.35 color=\"black\"]\n"
                           "  \"Chasm: The Rift\" -- \"Serious Sam: Siberian Mayhem\" [label=0.3 color=\"black\"]\n"
                           "  \"Chasm: The Rift\" -- \"CARNAL\" [label=0.25 color=\"black\"]\n"
                           "  \"Chasm: The Rift\" -- \"Nightmare Reaper\" [label=0.25 color=\"black\"]\n"
                           "  \"The Indie Game Legend 3D\" -- \"Square Head Zombies 2 - FPS Game\" [label=0.35 color=\"black\"]\n"
                           "  \"The Indie Game Legend 3D\" -- \"Hellscreen\" [label=0.35 color=\"black\"]\n"
                           "  \"The Indie Game Legend 3D\" -- \"Prodeus\" [label=0.35 color=\"black\"]\n"
                           "  \"The Indie Game Legend 3D\" -- \"Serious Sam Classic: The First Encounter\" [label=0.35 color=\"black\"]\n"
                           "  \"The Indie Game Legend 3D\" -- \"Square Head Zombies - FPS Game\" [label=0.35 color=\"black\"]\n"
                           "  \"Dead Beacon\" -- \"Square Head Zombies 2 - FPS Game\" [label=0.35 color=\"black\"]\n"
                           "  \"Dead Beacon\" -- \"Square Head Zombies - FPS Game\" [label=0.3 color=\"black\"]\n"
                           "  \"Dead Beacon\" -- \"Nightmare of Decay\" [label=0.210526 color=\"black\"]\n"
                           "  \"Dead Beacon\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"Nightmare of Decay\" -- \"Dead Beacon\" [label=0.210526 color=\"black\"]\n"
                           "  \"Nightmare of Decay\" -- \"Invention 3\" [label=0.243243 color=\"black\"]\n"
                           "  \"Square Head Zombies 2 - FPS Game\" -- \"Square Head Zombies - FPS Game\" [label=0.1 color=\"black\"]\n"
                           "  \"Square Head Zombies 2 - FPS Game\" -- \"Sass VS Fash: Girlballs of Steel\" [label=0.35 color=\"black\"]\n"
                           "  \"Square Head Zombies 2 - FPS Game\" -- \"Dead Beacon\" [label=0.35 color=\"black\"]\n"
                           "  \"Square Head Zombies 2 - FPS Game\" -- \"Rise Of A Hero\" [label=0.315789 color=\"black\"]\n"
                           "  \"Square Head Zombies 2 - FPS Game\" -- \"Zombaliens\" [label=0.333333 color=\"black\"]\n"
                           "  \"Square Head Zombies 2 - FPS Game\" -- \"The Indie Game Legend 3D\" [label=0.35 color=\"black\"]\n"
                           "  \"Prodeus\" -- \"The Indie Game Legend 3D\" [label=0.35 color=\"black\"]\n"
                           "  \"Invention 3\" -- \"Nightmare of Decay\" [label=0.243243 color=\"black\"]\n"
                           "  \"Invention 3\" -- \"Sinister Halloween\" [label=0.179487 color=\"black\"]\n"
                           "  \"Invention 3\" -- \"Cold Cable: Lifeshift\" [label=0.179487 color=\"black\"]\n"
                           "  \"Invention 3\" -- \"Death Upon Us\" [label=0.230769 color=\"black\"]\n"
                           "  \"Invention 3\" -- \"Take Care VR\" [label=0.25 color=\"black\"]\n"
                           "  \"Invention 3\" -- \"Death Slayer V\" [label=0.166667 color=\"black\"]\n"
                           "  \"Invention 3\" -- \"L.S.S II\" [label=0.230769 color=\"black\"]\n"
                           "  \"Rise Of A Hero\" -- \"Square Head Zombies 2 - FPS Game\" [label=0.315789 color=\"black\"]\n"
                           "  \"Arthurian Legends\" -- \"The Citadel\" [label=0.35 color=\"black\"]\n"
                           "  \"Nightmare Reaper\" -- \"Chasm: The Rift\" [label=0.25 color=\"black\"]\n"
                           "  \"Nightmare Reaper\" -- \"KARM\" [label=0.3 color=\"black\"]\n"
                           "  \"Ion Fury\" -- \"The Citadel\" [label=0.35 color=\"black\"]\n"
                           "  \"Ion Fury\" -- \"DOOM 64\" [label=0.25 color=\"black\"]\n"
                           "  \"Dark Places\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"KARM\" -- \"Conviction\" [label=0.272727 color=\"black\"]\n"
                           "  \"KARM\" -- \"Chasm: The Rift\" [label=0.25 color=\"black\"]\n"
                           "  \"KARM\" -- \"Forgive Me Father 2\" [label=0.25 color=\"black\"]\n"
                           "  \"KARM\" -- \"Dino Trauma\" [label=0.35 color=\"black\"]\n"
                           "  \"KARM\" -- \"The Citadel\" [label=0.35 color=\"black\"]\n"
                           "  \"KARM\" -- \"Nightmare Reaper\" [label=0.3 color=\"black\"]\n"
                           "  \"KARM\" -- \"CULTIC\" [label=0.263158 color=\"black\"]\n"
                           "  \"KARM\" -- \"SUFFER 2\" [label=0.3 color=\"black\"]\n"
                           "  \"KARM\" -- \"HROT\" [label=0.3 color=\"black\"]\n"
                           "  \"KARM\" -- \"Blood: Fresh Supply\" [label=0.3 color=\"black\"]\n"
                           "  \"Six Meat Under\" -- \"Forgive Me Father 2\" [label=0.222222 color=\"black\"]\n"
                           "  \"The Citadel\" -- \"Dino Trauma\" [label=0.35 color=\"black\"]\n"
                           "  \"The Citadel\" -- \"Chasm: The Rift\" [label=0.35 color=\"black\"]\n"
                           "  \"The Citadel\" -- \"KARM\" [label=0.35 color=\"black\"]\n"
                           "  \"The Citadel\" -- \"DOOM DAY\" [label=0.35 color=\"black\"]\n"
                           "  \"The Citadel\" -- \"Ion Fury\" [label=0.35 color=\"black\"]\n"
                           "  \"The Citadel\" -- \"Zombies Must Die\" [label=0.35 color=\"black\"]\n"
                           "  \"The Citadel\" -- \"Arthurian Legends\" [label=0.35 color=\"black\"]\n"
                           "  \"The Citadel\" -- \"Square Head Zombies - FPS Game\" [label=0.35 color=\"black\"]\n"
                           "  \"Square Head Zombies - FPS Game\" -- \"The Citadel\" [label=0.35 color=\"black\"]\n"
                           "  \"Square Head Zombies - FPS Game\" -- \"Square Head Zombies 2 - FPS Game\" [label=0.1 color=\"black\"]\n"
                           "  \"Square Head Zombies - FPS Game\" -- \"The Indie Game Legend 3D\" [label=0.35 color=\"black\"]\n"
                           "  \"Square Head Zombies - FPS Game\" -- \"Sass VS Fash: Girlballs of Steel\" [label=0.3 color=\"black\"]\n"
                           "  \"Square Head Zombies - FPS Game\" -- \"Dead Beacon\" [label=0.3 color=\"black\"]\n"
                           "  \"IMHA\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"Hellscreen\" -- \"Forgive Me Father 2\" [label=0.3 color=\"black\"]\n"
                           "  \"Hellscreen\" -- \"The Indie Game Legend 3D\" [label=0.35 color=\"black\"]\n"
                           "  \"Zombies Must Die\" -- \"The Citadel\" [label=0.35 color=\"black\"]\n"
                           "  \"Zombies Must Die\" -- \"FOREST OF THE DEAD\" [label=0.243243 color=\"black\"]\n"
                           "  \"Zombies Must Die\" -- \"Samozbor ID:HEAVEN\" [label=0.3 color=\"black\"]\n"
                           "  \"CULTIC\" -- \"Forgive Me Father 2\" [label=0.263158 color=\"black\"]\n"
                           "  \"CULTIC\" -- \"KARM\" [label=0.263158 color=\"black\"]\n"
                           "  \"Dark Invasion VR: Doomsday\" -- \"L.S.S II\" [label=0.3 color=\"black\"]\n"
                           "  \"Zombaliens\" -- \"Square Head Zombies 2 - FPS Game\" [label=0.333333 color=\"black\"]\n"
                           "  \"Blood: Fresh Supply\" -- \"KARM\" [label=0.3 color=\"black\"]\n"
                           "  \"Blood: Fresh Supply\" -- \"Operation Body Count\" [label=0.210526 color=\"black\"]\n"
                           "  \"Blood: Fresh Supply\" -- \"DOOM 64\" [label=0.25 color=\"black\"]\n"
                           "  \"Blood: Fresh Supply\" -- \"Blood: One Unit Whole Blood\" [label=0.25 color=\"black\"]\n"
                           "  \"CARNAL\" -- \"Chasm: The Rift\" [label=0.25 color=\"black\"]\n"
                           "  \"Samozbor ID:HEAVEN\" -- \"Forgive Me Father 2\" [label=0.3 color=\"black\"]\n"
                           "  \"Samozbor ID:HEAVEN\" -- \"Zombies Must Die\" [label=0.3 color=\"black\"]\n"
                           "  \"Samozbor ID:HEAVEN\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"Nightmare Manor\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"Conviction\" -- \"KARM\" [label=0.272727 color=\"black\"]\n"
                           "  \"Sass VS Fash: Girlballs of Steel\" -- \"Dino Trauma\" [label=0.35 color=\"black\"]\n"
                           "  \"Sass VS Fash: Girlballs of Steel\" -- \"Square Head Zombies 2 - FPS Game\" [label=0.35 color=\"black\"]\n"
                           "  \"Sass VS Fash: Girlballs of Steel\" -- \"Square Head Zombies - FPS Game\" [label=0.3 color=\"black\"]\n"
                           "  \"MUTATION PHASE\" -- \"Dino Trauma\" [label=0.315789 color=\"black\"]\n"
                           "  \"MUTATION PHASE\" -- \"L.S.S II\" [label=0.263158 color=\"black\"]\n"
                           "  \"DOOM DAY\" -- \"The Citadel\" [label=0.35 color=\"black\"]\n"
                           "  \"The Story of Henry Bishop\" -- \"Cold Cable: Lifeshift\" [label=0.25 color=\"black\"]\n"
                           "  \"Forgive Me Father 2\" -- \"KARM\" [label=0.25 color=\"black\"]\n"
                           "  \"Forgive Me Father 2\" -- \"Six Meat Under\" [label=0.222222 color=\"black\"]\n"
                           "  \"Forgive Me Father 2\" -- \"Hellscreen\" [label=0.3 color=\"black\"]\n"
                           "  \"Forgive Me Father 2\" -- \"CULTIC\" [label=0.263158 color=\"black\"]\n"
                           "  \"Forgive Me Father 2\" -- \"Samozbor ID:HEAVEN\" [label=0.3 color=\"black\"]\n"
                           "  \"Forgive Me Father 2\" -- \"Forgive Me Father\" [label=0.3 color=\"black\"]\n"
                           "  \"Forgive Me Father 2\" -- \"Dino Trauma\" [label=0.35 color=\"black\"]\n"
                           "  \"Dino Trauma\" -- \"Forgive Me Father\" [label=0.3 color=\"black\"]\n"
                           "  \"Dino Trauma\" -- \"Sass VS Fash: Girlballs of Steel\" [label=0.35 color=\"black\"]\n"
                           "  \"Dino Trauma\" -- \"MUTATION PHASE\" [label=0.315789 color=\"black\"]\n"
                           "  \"Dino Trauma\" -- \"KARM\" [label=0.35 color=\"black\"]\n"
                           "  \"Dino Trauma\" -- \"Forgive Me Father 2\" [label=0.35 color=\"black\"]\n"
                           "  \"Dino Trauma\" -- \"Retchid\" [label=0.35 color=\"black\"]\n"
                           "  \"Dino Trauma\" -- \"The Citadel\" [label=0.35 color=\"black\"]\n"
                           "  \"Cold Cable: Lifeshift\" -- \"Invention 3\" [label=0.179487 color=\"black\"]\n"
                           "  \"Cold Cable: Lifeshift\" -- \"The Story of Henry Bishop\" [label=0.25 color=\"black\"]\n"
                           "  \"Cold Cable: Lifeshift\" -- \"Sinister Halloween\" [label=0.25 color=\"black\"]\n"
                           "  \"Cold Cable: Lifeshift\" -- \"Death Upon Us\" [label=0.25 color=\"black\"]\n"
                           "  \"Cold Cable: Lifeshift\" -- \"L.S.S II\" [label=0.3 color=\"black\"]\n"
                           "  \"Death Upon Us\" -- \"Cold Cable: Lifeshift\" [label=0.25 color=\"black\"]\n"
                           "  \"Death Upon Us\" -- \"Death Slayer V\" [label=0.189189 color=\"black\"]\n"
                           "  \"Death Upon Us\" -- \"Invention 3\" [label=0.230769 color=\"black\"]\n"
                           "  \"Death Upon Us\" -- \"L.S.S II\" [label=0.3 color=\"black\"]\n"
                           "  \"Death Slayer V\" -- \"Death Upon Us\" [label=0.189189 color=\"black\"]\n"
                           "  \"Death Slayer V\" -- \"Invention 3\" [label=0.166667 color=\"black\"]\n"
                           "  \"Take Care VR\" -- \"Invention 3\" [label=0.25 color=\"black\"]\n"
                           "  \"Forgive Me Father\" -- \"Forgive Me Father 2\" [label=0.3 color=\"black\"]\n"
                           "  \"Forgive Me Father\" -- \"Dino Trauma\" [label=0.3 color=\"black\"]\n"
                           "  \"L.S.S II\" -- \"Invention 3\" [label=0.230769 color=\"black\"]\n"
                           "  \"L.S.S II\" -- \"Dark Invasion VR: Doomsday\" [label=0.3 color=\"black\"]\n"
                           "  \"L.S.S II\" -- \"Survive and Revenge\" [label=0.3 color=\"black\"]\n"
                           "  \"L.S.S II\" -- \"MUTATION PHASE\" [label=0.263158 color=\"black\"]\n"
                           "  \"L.S.S II\" -- \"ETERNAL BLOOD\" [label=0.3 color=\"black\"]\n"
                           "  \"L.S.S II\" -- \"Project RIP\" [label=0.3 color=\"black\"]\n"
                           "  \"L.S.S II\" -- \"Death Upon Us\" [label=0.3 color=\"black\"]\n"
                           "  \"L.S.S II\" -- \"Swords with spice\" [label=0.3 color=\"black\"]\n"
                           "  \"L.S.S II\" -- \"Cold Cable: Lifeshift\" [label=0.3 color=\"black\"]\n"
                           "  \"L.S.S II\" -- \"Zombie Defense\" [label=0.3 color=\"black\"]\n"
                           "  \"L.S.S II\" -- \"FOREST OF THE DEAD\" [label=0.243243 color=\"black\"]\n"
                           "  \"Survive and Revenge\" -- \"L.S.S II\" [label=0.3 color=\"black\"]\n"
                           "  \"Swords with spice\" -- \"L.S.S II\" [label=0.3 color=\"black\"]\n"
                           "  \"Zombie Defense\" -- \"L.S.S II\" [label=0.3 color=\"black\"]\n"
                           "  \"DOOM Eternal\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"FOREST OF THE DEAD\" -- \"L.S.S II\" [label=0.243243 color=\"black\"]\n"
                           "  \"FOREST OF THE DEAD\" -- \"Zombies Must Die\" [label=0.243243 color=\"black\"]\n"
                           "  \"DOOM II\" -- \"Final DOOM\" [label=0.1 color=\"black\"]\n"
                           "  \"DOOM II\" -- \"Ultimate Doom\" [label=0.15 color=\"black\"]\n"
                           "  \"DOOM II\" -- \"DOOM 64\" [label=0.2 color=\"black\"]\n"
                           "  \"DOOM II\" -- \"Master Levels for Doom II\" [label=0.157895 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"Samozbor ID:HEAVEN\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"Nightmare Manor\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"Dead Beacon\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"Hellbound\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"DOOM Eternal\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"Devil Daggers\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"PERISH\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"Dark Places\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"Forkscythe\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"This Strange Realm Of Mine\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"Hellbound: Survival Mode\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"IMHA\" [label=0.3 color=\"black\"]\n"
                           "  \"Bloodhound\" -- \"Serious Sam: Siberian Mayhem\" [label=0.3 color=\"black\"]\n"
                           "  \"Hellbound\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"Retchid\" -- \"Dino Trauma\" [label=0.35 color=\"black\"]\n"
                           "  \"Devil Daggers\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"PERISH\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"This Strange Realm Of Mine\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"Ultimate Doom\" -- \"DOOM II\" [label=0.15 color=\"black\"]\n"
                           "  \"Ultimate Doom\" -- \"Master Levels for Doom II\" [label=0.105263 color=\"black\"]\n"
                           "  \"Ultimate Doom\" -- \"DOOM 64\" [label=0.15 color=\"black\"]\n"
                           "  \"Ultimate Doom\" -- \"Final DOOM\" [label=0.15 color=\"black\"]\n"
                           "  \"Hellbound: Survival Mode\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"Serious Sam: Siberian Mayhem\" -- \"Bloodhound\" [label=0.3 color=\"black\"]\n"
                           "  \"Serious Sam: Siberian Mayhem\" -- \"Chasm: The Rift\" [label=0.3 color=\"black\"]\n"
                           "  \"ETERNAL BLOOD\" -- \"L.S.S II\" [label=0.3 color=\"black\"]\n"
                           "  \"SUFFER 2\" -- \"KARM\" [label=0.3 color=\"black\"]\n"
                           "  \"HROT\" -- \"KARM\" [label=0.3 color=\"black\"]\n"
                           "  \"Conviction\" -- \"KARM\" [color=red]\n"
                           "  \"KARM\" -- \"Forgive Me Father 2\" [color=red]\n"
                           "  \"Forgive Me Father 2\" -- \"Samozbor ID:HEAVEN\" [color=red]\n"
                           "  \"Samozbor ID:HEAVEN\" -- \"Bloodhound\" [color=red]\n"
                           "  \"Bloodhound\" -- \"DOOM Eternal\" [color=red]\n"
                           "}";
    //init(parser, graph, games, keys);
    std::string curl_command = "curl -X POST -F \"dot=@graph.dot\" https://dreampuf.github.io/GraphvizOnline/render";
    int result = system(curl_command.c_str());
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
