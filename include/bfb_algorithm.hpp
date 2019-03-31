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
    private:
        vector<Segment> allSegments;
        vector<Vertex> resultPath;
        double resultCost;
        int observedLen;
        char baseDir;
        double haploidDepth;
        unordered_map<int,pair<Segment*,Segment*>> vertexOrderMap;
        unordered_map<int,unordered_map<int,array<array<Edge*,2>,2>>> adjacentMatrix;
    public:
        BFBAlgorithm(Graph g);
        vector<Vertex> createBase();
        vector<int> calculateCandidateArmLens(Vertex candidate,vector<Vertex> BFBPath, vector<vector<int>> allArmLens);
        void BFBTraverse(vector<Vertex> path,vector<vector<int>> allArmLens,double loss);
        vector<Vertex>  getCandidates(Vertex lastVertex);
        void generateAdjacentMatrix(Segment sourceSegment, Segment targetSegment);
        bool BFBTraverseUtil();
        void printResult();
};
#endif /* bfb_algorithm_hpp */
