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
#include "Graph.hpp"
using namespace std;

class BFBAlgorithm {
    private:
        vector<Segment> allSegments;
        int observedLen;
        unordered_map<int,pair<Segment*,Segment*>> vertexOrderMap;
    public:
        BFBAlgorithm(Graph g);
        vector<Vertex> createBase();
        vector<int> calculateCandidateArmLens(Vertex candidate,vector<Vertex> BFBPath, vector<vector<int>> allArmLens);
        bool BFBTraverse(vector<Vertex> path,vector<vector<int>> allArmLens);
        vector<Vertex> getCandidates(Vertex lastVertex);
        Edge* getConnectedEdge(Vertex source, Vertex target);
        bool BFBTraverseUtil();
        bool isSymmetric(Vertex former, Vertex candidate);
};
#endif /* bfb_algorithm_hpp */
