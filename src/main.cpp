//
//  main.cpp
//  BFBDetectGraph
//
//  Created by Tidesun on 21/3/2019.
//  Copyright © 2019 Oliver. All rights reserved.
//

#include <iostream>
#include "bfb_algorithm.hpp"
using namespace std;
int main(int argc, char *argv[]) {
    Graph g = Graph();
    g.readGraph("../static/sim.lh");
    g.calculateCopyNum();
    BFBAlgorithm bfbAlgorithm = BFBAlgorithm(g,0.1);
    bfbAlgorithm.BFBTraverseUtil();
    return 0;
}