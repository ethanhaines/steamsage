cmake_minimum_required(VERSION 3.29)
project(steamsage CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_SOURCE_DIR}/include)




# SFML link https://www.sfml-dev.org/download/sfml/2.6.2/
# download "Visual C++ 17 (2022) - 64-bit"
include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.2)
FETCHCONTENT_MAKEAVAILABLE(SFML)

# set(SFML_DIR "C:/SFML-2.6.2/lib/cmake/SFML") # CHANGE THIS TO "path/to/your/sfml/lib/cmake/SFML"
# set(SFML_STATIC_LIBRARIES ON)
# find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
# find_package(OpenAL)


# here's the links
# https://graphviz.org/download/ download by clicking "graphviz-12.2.0 (64-bit) EXE installer [sha256]"
# how to install https://forum.graphviz.org/t/new-simplified-installation-procedure-on-windows/224

# MAKE SURE YOU RESTART YOUR IDE
include_directories("../libs/include")
link_directories("../libs/lib")

# link_directories(C:/SFML-2.6.2/lib)  # Add this if SFML libraries are not found


add_executable(steamsage
        src/AdjacencyList.cpp
        src/Parser.cpp
        include/AdjacencyList.h
        include/Parser.h
        include/csv.h
)

target_link_libraries(steamsage sfml-graphics)