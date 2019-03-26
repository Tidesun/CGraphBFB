cmake_minimum_required(VERSION 3.13)
project(BFBGraph)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(BFBGraph
        include/bfb_algorithm.hpp
        include/Edge.hpp
        include/Exceptions.hpp
        include/Graph.hpp
        include/Junction.hpp
        include/Segment.hpp
        include/Vertex.hpp
        include/Weight.hpp
        src/bfb_algorithm.cpp
        src/Edge.cpp
        src/Exceptions.cpp
        src/Graph.cpp
        src/Junction.cpp
        src/main.cpp
        src/Segment.cpp
        src/Vertex.cpp
        src/Weight.cpp
        main.cpp)
