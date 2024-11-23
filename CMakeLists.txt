cmake_minimum_required(VERSION 3.29)
project(steamsage CXX)

set(CMAKE_C_STANDARD 17)

include_directories(.)
include_directories(include)
include_directories(resources)

add_executable(steamsage
        src/main.cpp
        src/AdjacencyMatrix.cpp
        src/Parser.cpp
        resources/AdjacencyMatrix.h
        resources/Parser.h)

include_directories("C:/Program Files/PostgreSQL/17/include")
link_directories("C:/Program Files/PostgreSQL/17/lib")
target_link_libraries(steamsage pq)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L\"C:/Program Files/PostgreSQL/17/lib\" -lpq")
