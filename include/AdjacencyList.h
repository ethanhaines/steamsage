
#ifndef ADJACENCYMATRIX_H
#define ADJACENCYMATRIX_H

#endif //ADJACENCYMATRIX_H

#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>
class AdjList {
public:
    void insert(const std::string& from, const std::string& to, const std::vector<std::string>& from_tags, const std::vector<std::string>& to_tags);
    void print();
private:
    std::unordered_map<std::string, std::vector<std::string>> adjlist;
    std::unordered_map<std::string, std::unordered_set<std::string>> tags;
};