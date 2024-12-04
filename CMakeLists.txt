cmake_minimum_required(VERSION 3.29)
project(steamsage CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(steamsage
        src/main.cpp
        src/AdjacencyList.cpp
        src/Parser.cpp
        include/AdjacencyList.h
        include/Parser.h
        include/csv.h
)


