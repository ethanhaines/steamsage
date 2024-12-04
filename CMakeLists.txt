cmake_minimum_required(VERSION 3.29)
project(steamsage CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_SOURCE_DIR}/include)


# here's the links
# https://graphviz.org/download/ download by clicking "graphviz-12.2.0 (64-bit) EXE installer [sha256]"
# how to install https://forum.graphviz.org/t/new-simplified-installation-procedure-on-windows/224

# MAKE SURE YOU RESTART YOUR IDE

include_directories("../libs/include")
link_directories("../libs/lib")

add_executable(steamsage
        src/main.cpp
        src/AdjacencyList.cpp
        src/Parser.cpp
        include/AdjacencyList.h
        include/Parser.h
        include/csv.h
)


