//
// Created by Tidesun on 2019-04-25.
//

#include "bfb_calculator.hpp"
BFBCalculator::BFBCalculator(Graph _g,double _costThreshold, char _baseDir, char _extendingDir){
    algorithm = nullptr;
    g = _g;
    costThreshold = _costThreshold;
    baseDir = _baseDir;
    extendingDir = _extendingDir;
}
void BFBCalculator::setAlgorithm(int type) {
    if (type == foldbackAlgo) {
        algorithm = new FoldbackAlgorithm(&g,costThreshold, baseDir, extendingDir);
    }
    if (type == traverseAlgo) {
        algorithm = new TraverseAlgorithm(&g,costThreshold, baseDir, extendingDir);
    }
}
void BFBCalculator::runAlgorithm() {
    g.backupCopy();
    if (!algorithm) {
        cout<< "Set algorithm first!";
        return;
    }
    auto start = std::chrono::system_clock::now();
    algorithm->traverseUtil();
    algorithm->printResult();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    g.restoreCopy();
}