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
//BAABCCBAAAABC
    Graph g = Graph();
    g.addSegment(1,3,1);
    g.addSegment(2,4,1);
    g.addSegment(3,6,1);
    g.addJunction(1,'+',2,'+',3,1);
    g.addJunction(2,'+',3,'+',4,1);
    g.addJunction(3,'+',3,'-',3,1);
    g.addJunction(3,'-',3,'+',1,1);
    g.addJunction(1,'-',1,'+',1,1);
    BFBAlgorithm bfbAlgo = BFBAlgorithm(g);
    bfbAlgo.BFBTraverseUtil();
    return 0;
}
