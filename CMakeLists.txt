cmake_minimum_required(VERSION 3.29)
project(steamsage CXX)

set(CMAKE_C_STANDARD 17)

add_executable(steamsage
        src/main.cpp
        src/AdjacencyMatrix.cpp
        src/Parser.cpp
        include/AdjacencyMatrix.h
        include/Parser.h)

