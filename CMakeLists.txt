cmake_minimum_required(VERSION 3.29)
project(steamsage CXX)

set(CMAKE_C_STANDARD 14)

include_directories(.)
include_directories(include)
include_directories(resources)

add_executable(steamsage
        src/main.cpp
        src/AdjacencyMatrix.cpp
        src/Parser.cpp
        resources/AdjacencyMatrix.h
        resources/Parser.h)
