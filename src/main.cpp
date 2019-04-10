//
//  main.cpp
//  BFBDetectGraph
//
//  Created by Tidesun on 21/3/2019.
//  Copyright © 2019 Oliver. All rights reserved.
//

#include <iostream>
#include "bfb_algorithm.hpp"
#include <chrono>
#include <ctime>
using namespace std;
int main(int argc, char *argv[]) {
    double costThreshold = 0.0;
    if (argc>1) {
     costThreshold = atof(argv[1]);
    }
    Graph g = Graph();
    g.readGraph("../static/sim.lh");
    g.calculateCopyNum();
    BFBAlgorithm bfbAlgorithm = BFBAlgorithm(g,costThreshold);
    auto start = std::chrono::system_clock::now();
    bfbAlgorithm.BFBTraverseUtil();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
    return 0;
}