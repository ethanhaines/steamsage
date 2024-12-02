#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include "../include/Parser.h"
#include "../include/csv.h"

//this function is used to split the csv as it goes through line by line
std::vector<std::string> Parser::delim_split(const std::string &str, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)){
        tokens.push_back(token);
    }
    return tokens;
}

std::unordered_map<std::string, std::vector<std::string>> Parser::parse() {
    std::unordered_map<std::string, std::vector<std::string>> data;
    io::CSVReader<2, io::trim_chars<' '>, io::double_quote_escape<',','\"'>> in(path);
    in.read_header(io::ignore_extra_column, "Name", "Tags");
    std::string name, tags;
    while (in.read_row(name, tags)) {
        if (tags.empty()){
            continue; // our program won't work with games with no tags, so exclude them (shouldn't be that many)
        }
        std::vector<std::string> tags_vector = delim_split(tags, ',');
        bool has_banned_tag = false;
        for (const auto& tag : tags_vector) {
            if (banned_words.find(tag) != banned_words.end()) {
                has_banned_tag = true;
                break;
            }
        }
        if (has_banned_tag) {
            continue; // skip this game if it contains a banned tag
        }
        data[name] = tags_vector;
    }
    game_data = data;
    return game_data;
}


