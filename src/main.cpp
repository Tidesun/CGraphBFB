//
//  main.cpp
//  BFBDetectGraph
//
//  Created by Tidesun on 21/3/2019.
//  Copyright © 2019 Oliver. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <ctime>
#include "file_reader.hpp"
#include "bfb_calculator.hpp"
using namespace std;
int main(int argc, char *argv[]) {
    double costThreshold = 0.0;
    if (argc>1) {
        costThreshold = atof(argv[1]);
    }
    FileReader fileReader = FileReader("../static/vcf.csv",FileReader::SVCN);
    //FileReader fileReader = FileReader("../static/sim.lh",FileReader::GRAPH);
    Graph g = Graph();
    fileReader.readGraph(&g);
    BFBCalculator cal = BFBCalculator(g,costThreshold,'+','r');
    cal.setAlgorithm(BFBCalculator::foldbackAlgo);
    cal.runAlgorithm();
//    cal.setAlgorithm(BFBCalculator::traverseAlgo);
//    cal.runAlgorithm();
    return 0;
}