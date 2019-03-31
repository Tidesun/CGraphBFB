//
//  bfb_algorithm.cpp
//  BFBDetectGraph
//
//  Created by Tidesun on 25/3/2019.
//  Copyright © 2019 Oliver. All rights reserved.
//

#include "bfb_algorithm.hpp"
using namespace std;
BFBAlgorithm::BFBAlgorithm(Graph g){
    double cnSum = 0.0;
    for (auto const & segment:*g.getSegments()){
        allSegments.push_back(*segment);
        cnSum += segment->getWeight()->getCopyNum();
    }
    resultCost = cnSum + 1;
    //observedLen = (int) cnSum;
    observedLen = 170;
    baseDir = '+';
    haploidDepth = g.getAvgRawCoverage()/g.getAvgPloidy();
}
vector<int> BFBAlgorithm::calculateCandidateArmLens(Vertex candidate,vector<Vertex> BFBPath, vector<vector<int>> allArmLens){
    vector<int> candidateArmLens(1,0);
    // indicate whether a palindrome suffix is found if add candidate
    int lastPos = (int) allArmLens.size() - 1;
    vector<int> lastArmLens = allArmLens.back();
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
vector<Vertex> BFBAlgorithm::createBase(){
    //TODO: create reference segments
    vector<Vertex> base;
    for (int i=0;i<allSegments.size();i++) {
        base.push_back(*allSegments[i].getPositiveVertex());
        if (i > 0) {
            vertexOrderMap[allSegments[i].getId()].first = &allSegments[i-1];
            generateAdjacentMatrix(allSegments[i],allSegments[i-1]);
        }
        if (i < allSegments.size() - 1) {
            vertexOrderMap[allSegments[i].getId()].second = &allSegments[i+1];
            generateAdjacentMatrix(allSegments[i],allSegments[i+1]);
        }
    }
    return base;
}
void BFBAlgorithm::BFBTraverse(vector<Vertex> path,vector<vector<int>> allArmLens,double cost){
    // if a better solution is found
    if (path.size()== observedLen && cost<resultCost) {
        resultPath = path;
        resultCost = cost;
        cout<< resultCost<< endl;
        return;
    }
    // if the path is too long or cost has exceeded the best cost
    if (path.size() >= observedLen || cost>=resultCost){
        return;
    }
    Vertex lastVertex = path.back();
    vector<Vertex> candidates = getCandidates(lastVertex);
    for (auto & candidate:candidates) {
        vector<int> candidateArmLens = calculateCandidateArmLens(candidate,path,allArmLens);
        // if the candidate can form a palindrome
        if (candidateArmLens.size()>1) {
            double tempCost = cost;
            allArmLens.push_back(candidateArmLens);
            path.push_back(candidate);
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

            connectedEdge->traverse();
            BFBTraverse(path,allArmLens,tempCost);
            allArmLens.pop_back();
            path.pop_back();
            connectedEdge->recover();
        }
    }

}
bool BFBAlgorithm::BFBTraverseUtil() {
    vector<vector<int>> allArmLens(allSegments.size(),vector<int>(1,0));
    vector<Vertex> path = createBase();
    BFBTraverse(path,allArmLens,0.0);
    if (!resultPath.empty())
        return true;
    return false;
}
void BFBAlgorithm::printResult() {
    string res;
    for (auto & vertex: resultPath) {
        res += vertex.getInfo() + ' ';
    }
    cout<<res<<endl;
    cout<< resultCost<<endl;
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
        candidates.push_back(*candidateVertex);
    }
    candidates.push_back(*lastVertex.getComplementVertex());
    return candidates;
}
void BFBAlgorithm::generateAdjacentMatrix(Segment sourceSegment, Segment targetSegment) {
    for (auto source: {*sourceSegment.getPositiveVertex(),*sourceSegment.getNegativeVertex()})
        for (auto target: {*targetSegment.getPositiveVertex(),*targetSegment.getNegativeVertex()}) {
            int sourceDir = source.getDir() == '+' ? 0 : 1;
            int targetDir = source.getDir() == '+' ? 0 : 1;
            for (Edge *edge: *source.getEdgesAsSource()) {
                if (edge->getTarget()->getId() == target.getId() && edge->getTarget()->getDir() == target.getDir()) {
                    adjacentMatrix[source.getId()][target.getId()][sourceDir][targetDir] = edge;
                }
            };
        }
}