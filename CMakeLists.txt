cmake_minimum_required(VERSION 3.13)
project(BFBGraph)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

#find_package(Boost COMPONENTS random system thread filesystem chrono atomic date_time regex)
#find_package(cpprestsdk REQUIRED)
#include_directories(${Boost_INCLUDE_DIRS})

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
        main.cpp src/file_reader.cpp include/file_reader.hpp src/foldback_algorithm.cpp include/foldback_algorithm.hpp src/traverse_algorithm.cpp include/traverse_algorithm.hpp src/bfb_calculator.cpp include/bfb_calculator.hpp)
        #src/httphandler.cpp include/httphandler.h include/stdafx.h)
#target_link_libraries(BFBGraph ${Boost_LIBRARIES} cpprestsdk::cpprest)