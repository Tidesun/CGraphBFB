//
// Created by Tidesun on 2019-04-25.
//

#ifndef BFBGRAPH_FOLDBACK_ALGORITHM_HPP
#define BFBGRAPH_FOLDBACK_ALGORITHM_HPP
#include "bfb_algorithm.hpp"
using namespace std;

class FoldbackAlgorithm: public BFBAlgorithm {
    public:
        explicit FoldbackAlgorithm(Graph* g, double _costThreshold = 0.0,char _baseDir = '+',char _extendingDir = 'r'): BFBAlgorithm(g,_costThreshold,_baseDir,_extendingDir){}
        void traverse(Edge* lastFoldbackJunc, vector<Edge*> foldbackJuncs,vector<Vertex>* path,double cost);
        void traverseUtil() override;
        vector<Vertex*> getFoldbackConnectionVertices(Edge* sourceJunc,Edge* sinkJunc);
        bool isValidCandidate(Edge* sourceJunc,Edge* sinkJunc);
        bool isOppositeFoldbackJunction(Edge* sourceJunc,Edge* sinkJunc);
};


#endif //BFBGRAPH_FOLDBACK_ALGORITHM_HPP
