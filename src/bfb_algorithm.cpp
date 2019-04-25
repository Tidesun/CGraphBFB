//
//  bfb_algorithm.cpp
//  BFBDetectGraph
//
//  Created by Tidesun on 25/3/2019.
//  Copyright © 2019 Oliver. All rights reserved.
//
#include "bfb_algorithm.hpp"
using namespace std;
BFBAlgorithm::BFBAlgorithm(Graph* g, double _costThreshold = 0.0,char _baseDir = '+',char _extendingDir = 'r'){
    double cnSum = 0.0;
    for (auto const & segment:*g->getSegments()){
        allSegments.push_back(*segment);
        cnSum += segment->getWeight()->getCopyNum();
    }
    if (_extendingDir == 'l') {
        reverse(allSegments.begin(),allSegments.end());
        baseDir = _baseDir == '+' ? '-' : '+';
    } else {
        baseDir = _baseDir;
    }
    resultCost = cnSum + 1;
    observedLen = (int) cnSum;
    costThreshold = _costThreshold;
    extendingDir = _extendingDir;
    haploidDepth = g->getAvgRawCoverage()/g->getAvgPloidy();
}
vector<Vertex> BFBAlgorithm::createBase(double &cost){
    //TODO: create reference segments
    vector<Vertex> base;
    for (int i=0;i<allSegments.size();i++) {
        Vertex vertex = baseDir == '+' ? *allSegments[i].getPositiveVertex() : *allSegments[i].getNegativeVertex();
        base.push_back(vertex);
        if (i > 0) {
            vertexOrderMap[allSegments[i].getId()].first = &allSegments[i-1];
            Edge* edge = baseDir == '+' ? getConnectedEdge(*allSegments[i-1].getPositiveVertex(),*allSegments[i].getPositiveVertex(),cost)
                    : getConnectedEdge(*allSegments[i-1].getNegativeVertex(),*allSegments[i].getNegativeVertex(),cost);
            edge->traverse();
            generateAdjacentMatrix(allSegments[i],allSegments[i-1]);
        }
        if (i < allSegments.size() - 1) {
            vertexOrderMap[allSegments[i].getId()].second = &allSegments[i+1];
            generateAdjacentMatrix(allSegments[i],allSegments[i+1]);
        }
        generateAdjacentMatrix(allSegments[i],allSegments[i]);
    }
    return base;
}
void BFBAlgorithm::printResult() {
    if (resultPath.empty()){
        cout<<"No result obtained under the cost threshold "<<costThreshold<<endl;
        return;
    }
    string res;
    for (auto & vertex: resultPath) {
        res += vertex.getInfo() + " -> ";
    }
    cout<<res<<endl;
    cout<< resultCost<<endl;
    cout<< 1 - resultCost/observedLen<<endl;
}
void BFBAlgorithm::generateAdjacentMatrix(Segment sourceSegment, Segment targetSegment) {
    for (auto source: {*sourceSegment.getPositiveVertex(),*sourceSegment.getNegativeVertex()})
        for (auto target: {*targetSegment.getPositiveVertex(),*targetSegment.getNegativeVertex()}) {
            int sourceDir = source.getDir() == '+' ? 0 : 1;
            int targetDir = target.getDir() == '+' ? 0 : 1;
            for (Edge *edge: *source.getEdgesAsSource()) {
                if (edge->getTarget()->getId() == target.getId() && edge->getTarget()->getDir() == target.getDir()) {
                    adjacentMatrix[source.getId()][target.getId()][sourceDir][targetDir] = edge;
                }
            };
        }
}
Edge* BFBAlgorithm::getConnectedEdge(Vertex lastVertex,Vertex candidate,double &tempCost) {
    int sourceDir = lastVertex.getDir() == '+' ? 0: 1;
    int targetDir = candidate.getDir() == '+' ? 0: 1;
    Edge * connectedEdge = adjacentMatrix[lastVertex.getId()][candidate.getId()][sourceDir][targetDir];
    if (!connectedEdge || !connectedEdge->hasCopy()){
        tempCost += connectedEdge?1.0 - connectedEdge->getWeight()->getCopyNum():1.0;
        auto _junc =
                new Junction(lastVertex.getSegment(),candidate.getSegment(),lastVertex.getDir(),candidate.getDir(),1.0,1.0);
        double juncCopy = _junc->getWeight()->getCoverage() / haploidDepth;
        _junc->getWeight()->setCopyNum(max(juncCopy, 0.0));
        connectedEdge = _junc->getEdgeA();
    }
    return connectedEdge;
}