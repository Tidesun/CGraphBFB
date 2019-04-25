//
// Created by Tidesun on 2019-04-25.
//

#ifndef BFBGRAPH_BFB_CALCULATOR_HPP
#define BFBGRAPH_BFB_CALCULATOR_HPP
#include "bfb_algorithm.hpp"
#include "foldback_algorithm.hpp"
#include "traverse_algorithm.hpp"

class BFBCalculator {
private:
    Graph g;
    double costThreshold;
    char baseDir;
    char extendingDir;
    BFBAlgorithm *algorithm;

public:
    enum AlgorithmType
    {
        foldbackAlgo, traverseAlgo
    };
    BFBCalculator(Graph g,double costThreshold, char baseDir, char extendingDir);
    void setAlgorithm(int type);
    void runAlgorithm();
};


#endif //BFBGRAPH_BFB_CALCULATOR_HPP
