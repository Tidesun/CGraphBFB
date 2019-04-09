//
//  bfb_algorithm.cpp
//  BFBDetectGraph
//
//  Created by Tidesun on 25/3/2019.
//  Copyright © 2019 Oliver. All rights reserved.
//
#include "bfb_algorithm.hpp"
using namespace std;
BFBAlgorithm::BFBAlgorithm(Graph g, double _costThreshold = 0.0){
    double cnSum = 0.0;
    for (auto const & segment:*g.getSegments()){
        allSegments.push_back(*segment);
        cnSum += segment->getWeight()->getCopyNum();
    }
    resultCost = cnSum + 1;
    observedLen = (int) cnSum;
    costThreshold = _costThreshold;
    baseDir = '+';
    haploidDepth = g.getAvgRawCoverage()/g.getAvgPloidy();
}
vector<int> BFBAlgorithm::calculateCandidateArmLens(Vertex candidate,vector<Vertex> BFBPath, vector<int> lastArmLens){
    vector<int> candidateArmLens(1,0);
    // indicate whether a palindrome suffix is found if add candidate
    int lastPos = (int) BFBPath.size() - 1;
    // iterate over all palindromes suffix ended with last char
    for (auto armLen : lastArmLens){
        // if a symmetric segment to candidate is found
        Vertex former = BFBPath[lastPos-armLen*2];
        if (former.getId()==candidate.getId() && former.getDir() != candidate.getDir()) {
            candidateArmLens.push_back(armLen+1);
        }
    }
    return candidateArmLens;
}
vector<Vertex> BFBAlgorithm::createBase(double &cost){
    //TODO: create reference segments
    vector<Vertex> base;
    for (int i=0;i<allSegments.size();i++) {
        base.push_back(*allSegments[i].getPositiveVertex());
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
void BFBAlgorithm::BFBTraverse(vector<Vertex> path,vector<int> lastArmLens,double cost){
    // if a better solution is found
    if (path.size()== observedLen && cost<resultCost) {
        resultPath = path;
        resultCost = cost;
        string res;
        for (auto & vertex: resultPath) {
            res += vertex.getInfo() + ' ';
        }
        return;
    }
    // if the path is too long or cost has exceeded the best cost
//    if (path.size() >= observedLen){
    if (path.size() >= observedLen || cost>=resultCost || cost>costThreshold*observedLen){
        return;
    }
    Vertex lastVertex = path.back();
    vector<Vertex> candidates = getCandidates(lastVertex);
    for (auto & candidate:candidates) {
        vector<int> candidateArmLens = calculateCandidateArmLens(candidate,path,lastArmLens);
        // if the candidate can form a palindrome
        if (candidateArmLens.size()>1) {
            double tempCost = cost;
            path.push_back(candidate);
            Edge* connectedEdge = getConnectedEdge(lastVertex,candidate,tempCost);
            connectedEdge->traverse();
            BFBTraverse(path,candidateArmLens,tempCost);
            path.pop_back();
            connectedEdge->recover();
        }
    }

}
bool BFBAlgorithm::BFBTraverseUtil() {
    vector<int> lastArmLens(1,0);
    double cost = 0.0;
    vector<Vertex> path = createBase(cost);
    BFBTraverse(path,lastArmLens,cost);
    printResult();
}
void BFBAlgorithm::printResult() {
    if (resultPath.empty()){
        cout<<"No result obtained under the cost threshold "<<costThreshold<<endl;
        return;
    }
    string res;
    for (auto & vertex: resultPath) {
        res += vertex.getInfo() + ' ';
    }
    cout<<res<<endl;
    cout<< resultCost<<endl;
    cout<< 1 - resultCost/observedLen<<endl;
}
vector<Vertex> BFBAlgorithm::getCandidates(Vertex lastVertex) {
    vector<Vertex> candidates;
    //TODO: justify which segment id is the first in ref
    Segment* candidateSegment = nullptr;
    Vertex* candidateVertex = nullptr;
    bool foundSegment = false;
    if (lastVertex.getDir() != baseDir && vertexOrderMap[lastVertex.getId()].first) {
        candidateSegment = vertexOrderMap[lastVertex.getId()].first;
        foundSegment = true;
    } else if (lastVertex.getDir() == baseDir && vertexOrderMap[lastVertex.getId()].second){
        candidateSegment = vertexOrderMap[lastVertex.getId()].second;
        foundSegment = true;
    }
    if (foundSegment){
        if (lastVertex.getDir() == '+') {
            candidateVertex = candidateSegment -> getPositiveVertex();
        } else {
            candidateVertex = candidateSegment -> getNegativeVertex();
        }
        int sourceDir = lastVertex.getDir() == '+' ? 0: 1;
        int targetDir = candidateVertex->getDir() == '+' ? 0: 1;
        Edge * connectedEdge = adjacentMatrix[lastVertex.getId()][candidateVertex->getId()][sourceDir][targetDir];
        if (!connectedEdge || !connectedEdge->hasCopy()){
            candidates.push_back(*lastVertex.getComplementVertex());
            candidates.push_back(*candidateVertex);
        } else {
            candidates.push_back(*candidateVertex);
            candidates.push_back(*lastVertex.getComplementVertex());
        }
    } else
        candidates.push_back(*lastVertex.getComplementVertex());
    return candidates;
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