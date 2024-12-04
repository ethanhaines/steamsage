cmake_minimum_required(VERSION 3.29)
project(steamsage CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_SOURCE_DIR}/include)


# here's the links
# https://graphviz.org/download/ download by clicking "graphviz-12.2.0 (64-bit) EXE installer [sha256]"
# how to install https://forum.graphviz.org/t/new-simplified-installation-procedure-on-windows/224

include_directories("../libs")
link_directories("../libs")

add_executable(steamsage
        src/main.cpp
        src/AdjacencyList.cpp
        src/Parser.cpp
        include/AdjacencyList.h
        include/Parser.h
        include/csv.h
)


