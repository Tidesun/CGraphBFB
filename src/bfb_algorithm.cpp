//
//  bfb_algorithm.cpp
//  BFBDetectGraph
//
//  Created by Tidesun on 25/3/2019.
//  Copyright © 2019 Oliver. All rights reserved.
//

#include "bfb_algorithm.hpp"
using namespace std;
const char baseDir = '+';
BFBAlgorithm::BFBAlgorithm(Graph _g){
    g = &_g;
    double cnSum = 0.0;
    for (auto const & segment:*_g.getSegments()){
        allSegments.push_back(*segment);
        cnSum += segment->getWeight()->getCopyNum();
    }
    resultLoss = (numeric_limits<double>::max)();
    observedLen = (int) floor(cnSum);
}
vector<int> BFBAlgorithm::calculateCandidateArmLens(Vertex candidate,vector<Vertex> BFBPath, vector<vector<int>> allArmLens){
    vector<int> candidateArmLens(1,0);
    // indicate whether a palindrome suffix is found if add candidate
    int lastPos = (int) allArmLens.size() - 1;
    vector<int> lastArmLens = allArmLens.back();
    // iterate over all palindromes suffix ended with last char
    for (auto armLen : lastArmLens){
        // if a symmetric segment to candidate is found
        if (isSymmetric(BFBPath[lastPos-armLen*2],candidate)) {
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
        vertexOrderMap[allSegments[i].getId()].first = i > 0 ? &allSegments[i-1] : nullptr;
        vertexOrderMap[allSegments[i].getId()].second = i< allSegments.size()-1 ? &allSegments[i+1]: nullptr;
    }
    return base;
}
bool BFBAlgorithm::BFBTraverse(vector<Vertex> path,vector<vector<int>> allArmLens,double loss){
    if (path.size() > observedLen || loss>resultLoss){
        return false;
    }
    if (path.size()== observedLen && loss<resultLoss) {
        resultPath = path;
        resultLoss = loss;
        cout<< resultLoss<< endl;
        return true;
    }
    Vertex lastVertex = path.back();
    vector<Vertex> candidates = getCandidates(lastVertex);
    for (auto & candidate:candidates) {
        vector<int> candidateArmLens = calculateCandidateArmLens(candidate,path,allArmLens);
        // if the candidate can form a palindrome
        if (candidateArmLens.size()>1) {
            allArmLens.push_back(candidateArmLens);
            path.push_back(candidate);

            Edge * connectedEdge = getConnectedEdge(lastVertex,candidate);
            if (!connectedEdge || !connectedEdge->hasCopy()){
                loss += connectedEdge?1.0 - connectedEdge->getWeight()->getCopyNum():1.0;
                auto _junc =
                        new Junction(lastVertex.getSegment(),candidate.getSegment(),lastVertex.getDir(),candidate.getDir(),1.0,1.0);
                double juncCopy = _junc->getWeight()->getCoverage() / (g->getAvgRawCoverage()/g->getAvgPloidy());
                _junc->getWeight()->setCopyNum(max(juncCopy, 0.0));
                connectedEdge = _junc->getEdgeA();
            }

            connectedEdge->traverse();
            BFBTraverse(path,allArmLens,loss);
            allArmLens.pop_back();
            path.pop_back();
            connectedEdge->recover();
        }
    }
    return false;

}
bool BFBAlgorithm::BFBTraverseUtil() {
    vector<vector<int>> allArmLens(allSegments.size(),vector<int>(1,0));
    vector<Vertex> path = createBase();
    return BFBTraverse(path,allArmLens,0.0);
}
void BFBAlgorithm::getResult() {
    string res;
    for (auto & vertex: resultPath) {
        res += vertex.getInfo() + ' ';
    }
    cout<<res<<endl;
    cout<< resultLoss/observedLen<<endl;
}
bool BFBAlgorithm::isSymmetric(Vertex former, Vertex candidate) {
    return former.getId()==candidate.getId() && former.getDir() != candidate.getDir();
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
Edge* BFBAlgorithm::getConnectedEdge(Vertex source, Vertex target) {
//    cout<<source.getInfo();
    for (Edge* edge: *source.getEdgesAsSource()){
        if (edge->getTarget()->getId() == target.getId() && edge->getTarget()->getDir() == target.getDir()) {
            return edge;
        }
    };
    return nullptr;
}