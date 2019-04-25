//
//  bfb_algorithm.hpp
//  BFBDetectGraph
//
//  Created by Tidesun on 25/3/2019.
//  Copyright © 2019 Oliver. All rights reserved.
//

#ifndef bfb_algorithm_hpp
#define bfb_algorithm_hpp

#include <iostream>
#include <vector>
#include <unordered_map>
#include <array>
#include "Graph.hpp"
using namespace std;
class BFBAlgorithm {
protected:
    vector<Segment> allSegments;
    vector<Vertex> resultPath;
    double resultCost;
    int observedLen;
    char baseDir;
    char extendingDir;
    double haploidDepth;
    double costThreshold;
    unordered_map<int,pair<Segment*,Segment*>> vertexOrderMap;
    unordered_map<int,unordered_map<int,array<array<Edge*,2>,2>>> adjacentMatrix;
public:
    BFBAlgorithm(Graph* g, double _costThreshold,char _baseDir,char _extendingDir);
    ~BFBAlgorithm();
    vector<Vertex> createBase(double &cost);
    void generateAdjacentMatrix(Segment sourceSegment, Segment targetSegment);
    Edge* getConnectedEdge(Vertex lastVertex,Vertex candidate,double &tempCost);
    void printResult();
    virtual void traverseUtil() = 0;

};
#endif /* bfb_algorithm_hpp */
