//
//  main.cpp
//  BFBDetectGraph
//
//  Created by Tidesun on 21/3/2019.
//  Copyright © 2019 Oliver. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include "bfb_algorithm.hpp"
using namespace std;
int main() {
//    BFBAlgorithm bfbAlgo = BFBAlgorithm();
//    cout<<bfbAlgo.BFBTraverseUtil(2);
    Graph g = Graph();
    for (int i=0;i<10;i++) {
        g.addSegment(i,100,1);
        g.addJunction(i, '+', i, '-', i+i+1, 0.99);
    }
    for (int i=0;i<10;i++)
        for (int j=0;j<10;j++){
            g.addJunction(i, '+', j, '+', i+j+1, 0.99);
        }
    BFBAlgorithm bfbAlgo = BFBAlgorithm(g);
    bfbAlgo.BFBTraverseUtil();
    return 0;
}
