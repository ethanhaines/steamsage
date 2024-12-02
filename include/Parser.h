#ifndef PARSER_H
#define PARSER_H
#endif // PARSER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>

class Parser{
public:
    Parser(const std::string &path){ // path should be resources/games.csv
        this->path = path;
    }
    std::unordered_map<std::string, std::vector<std::string>> parse();
private:
    std::string path;
    //sadly, since steam has allowed porn games for probably over a year now, finding datasets without them is hard
    //so, I included this banned words list to prevent including them in this program
    //it may result in some false positives, like some GTA games have the nudity tag, but its hard to find a work around
    // the parsing still results in a list of 60,000 games, each with a name and tag list, so the data requirements should suffice
    std::unordered_set<std::string> banned_words = {"Hentai", "Nudity", "NSFW"};
    std::vector<std::string> delim_split(const std::string &str, char delim);
    std::unordered_map<std::string, std::vector<std::string>> game_data;
    std::unordered_map<std::string, int> tag_count;
};