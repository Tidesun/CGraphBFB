//
// Created by Tidesun on 2019-04-25.
//

#include "traverse_algorithm.hpp"
vector<int> TraverseAlgorithm::calculateCandidateArmLens(Vertex candidate,vector<Vertex> BFBPath, vector<int> lastArmLens){
    vector<int> candidateArmLens(1,0);
    // indicate whether a palindrome suffix is found if add candidate
    int lastPos = (int) BFBPath.size() - 1 ;
    // iterate over all palindromes suffix ended with last char
    for (auto armLen : lastArmLens){
        // if a symmetric segment to candidate is found
        if (lastPos-armLen*2 < 0) {
            break;
        }
        Vertex former = BFBPath[lastPos-armLen*2];
        if (former.getId()==candidate.getId() && former.getDir() != candidate.getDir()) {
            candidateArmLens.push_back(armLen+1);
        }
    }
    return candidateArmLens;
}
vector<Vertex> TraverseAlgorithm::getCandidates(Vertex lastVertex) {
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
void TraverseAlgorithm::traverse(vector <Vertex>* path,vector<int> lastArmLens,double cost){
    // if a better solution is found
    if (path->size()== observedLen && cost<resultCost) {
        resultPath = *path;
        if (extendingDir == 'l') {
            reverse(resultPath.begin(),resultPath.end());
            for (auto vertex=resultPath.begin();vertex!=resultPath.end();++vertex) {
                *vertex = *vertex->getComplementVertex();
            }
        }
        resultCost = cost;
        return;
    }
    // if the path is too long or cost has exceeded the best cost
    if (path->size() >= observedLen || cost>=resultCost || cost>costThreshold*observedLen){
        return;
    }
    Vertex lastVertex = path->back();
    vector<Vertex> candidates = getCandidates(lastVertex);
    for (auto & candidate:candidates) {
        vector<int> candidateArmLens = calculateCandidateArmLens(candidate,*path,lastArmLens);
        // if the candidate can form a palindrome
        if (candidateArmLens.size()>1) {
            double tempCost = cost;
            path->push_back(candidate);
            Edge* connectedEdge = getConnectedEdge(lastVertex,candidate,tempCost);
            connectedEdge->traverse();
            traverse(path,candidateArmLens,tempCost);
            path->pop_back();
            connectedEdge->recover();
        }
    }

}
void TraverseAlgorithm::traverseUtil() {
    vector<int> lastArmLens(1,0);
    double cost = 0.0;
    vector<Vertex> path = createBase(cost);
    traverse(&path,lastArmLens,cost);
}