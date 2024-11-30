cmake_minimum_required(VERSION 3.29)
project(steamsage CXX)

set(CMAKE_C_STANDARD 17)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(steamsage
        src/main.cpp
        src/AdjacencyList.cpp
        src/Parser.cpp
        include/AdjacencyList.h
        include/Parser.h
        src/BFS.cpp)

