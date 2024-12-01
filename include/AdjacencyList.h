
#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

#endif //ADJACENCYMATRIX_H

#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
class AdjList { // I got most of the logic from my pagerank project
public:
    void insert(const std::string& from, const std::string& to, const std::vector<std::string>& from_tags, const std::vector<std::string>& to_tags);
    void print();
    void BellmanFord();
    void Dijsktras();
    std::unordered_map<std::string, int> get_tag_count();
private:
    void calculate_weights(const std::string& from, const std::string& to, const std::vector<std::string>& from_tags, const std::vector<std::string>& to_tags);
    std::unordered_map<std::string, std::vector<std::pair<std::string, float>>> adjlist; // issue with this, we need to be able to know the weights of the specific edges e.g. V1-V2: 0.75
    std::unordered_map<std::string, std::unordered_set<std::string>> tags;
    std::unordered_map<std::string, int> tag_count;
};


