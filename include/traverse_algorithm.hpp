//
// Created by Tidesun on 2019-04-25.
//

#ifndef BFBGRAPH_TRAVERSE_ALGORITHM_HPP
#define BFBGRAPH_TRAVERSE_ALGORITHM_HPP
#include "bfb_algorithm.hpp"
using namespace std;
class TraverseAlgorithm: public BFBAlgorithm {
    public:
        explicit TraverseAlgorithm(Graph* g, double _costThreshold = 0.0,char _baseDir = '+',char _extendingDir = 'r'): BFBAlgorithm(g,_costThreshold,_baseDir,_extendingDir){}
        vector<int> calculateCandidateArmLens(Vertex candidate,vector<Vertex> BFBPath, vector<int> lastArmLens);
        void traverse(vector<Vertex>* path,vector<int> lastArmLens,double loss);
        vector<Vertex> getCandidates(Vertex lastVertex);
        void traverseUtil() override;
};


#endif //BFBGRAPH_TRAVERSE_ALGORITHM_HPP
