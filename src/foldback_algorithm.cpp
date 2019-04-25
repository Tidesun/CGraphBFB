//
// Created by Tidesun on 2019-04-25.
//

#include "foldback_algorithm.hpp"
void FoldbackAlgorithm::traverseUtil(){
    double cost = 0.0;
    vector<Edge*> foldbackJuncs;
    for (int i=0;i<allSegments.size();i++) {
        //TODO: open fold back gap / close fold back gap
//        foldbackJuncs.emplace_back(getConnectedEdge(*allSegments[i].getPositiveVertex(),*allSegments[i].getNegativeVertex(),cost));
//        foldbackJuncs.emplace_back(getConnectedEdge(*allSegments[i].getNegativeVertex(),*allSegments[i].getPositiveVertex(),cost));
        int segId = allSegments[i].getId();
        if (adjacentMatrix[segId][segId][0][1])
            foldbackJuncs.emplace_back(adjacentMatrix[segId][segId][0][1]);
        if (adjacentMatrix[segId][segId][1][0])
            foldbackJuncs.emplace_back(adjacentMatrix[segId][segId][1][0]);
    }
    cost = 0.0;
    // create a reference string
    vector<Vertex> path = createBase(cost);
    Vertex vertex = baseDir == '+' ? *allSegments.back().getNegativeVertex() : *allSegments.back().getPositiveVertex();
    Edge* lastFoldbackJunc =  getConnectedEdge(path.back(),vertex,cost);
    lastFoldbackJunc->traverse();
    path.push_back(vertex);
    traverse(lastFoldbackJunc,foldbackJuncs,&path,cost);
}
void FoldbackAlgorithm::traverse(Edge* lastFoldbackJunc, vector<Edge*> foldbackJuncs,vector<Vertex>* path,double cost){
    // if a better solution is found
    if (path->size() == observedLen && cost<resultCost) {
        resultPath = *path;
        resultCost = cost;
        if (extendingDir == 'l') {
            reverse(resultPath.begin(),resultPath.end());
            for (auto vertex=resultPath.begin();vertex!=resultPath.end();++vertex) {
                *vertex = *vertex->getComplementVertex();
            }
        }
        return;
    }
    // if the path is too long or cost has exceeded the best cost
    if (path->size() >= observedLen || cost>=resultCost || cost>costThreshold*observedLen){
        return;
    }
    for (auto candidate: foldbackJuncs){
        // if the candidate fold back junction is impossible
        if (!isValidCandidate(lastFoldbackJunc,candidate)){
            continue;
        }
        double tempCost = cost;
        // get the connection path between the foldback junction and candidate
        vector<Vertex*> tempVertices = getFoldbackConnectionVertices(lastFoldbackJunc,candidate);
        Vertex* prevVertex = lastFoldbackJunc->getTarget();
        // store the edges for recovering
        vector<Edge*> tempEdges;
        for (auto vertex: tempVertices) {
            // get the edge and modify the cost here
            Edge* edge = getConnectedEdge(*prevVertex,*vertex,tempCost);
            tempEdges.push_back(edge);
            edge->traverse();
            path->push_back(*vertex);
            prevVertex = vertex;
            /*
             if the path has reached the length stop extending the path
             the path do no always end with a fold back inversion
             */
            if (path->size() == observedLen)
                break;
        }
        traverse(candidate,foldbackJuncs,path,tempCost);
        for (auto edge: tempEdges) {
            edge->recover();
            path->pop_back();
        }
    }
}
bool FoldbackAlgorithm::isValidCandidate(Edge* sourceJunc,Edge* sinkJunc) {
    if (sourceJunc->getSource()->getDir() == sinkJunc->getSource()->getDir() && sourceJunc->getTarget()->getDir() == sinkJunc->getTarget()->getDir()) {
        return false;
    }
    // if the connection path between the fold-back junctions are impossible
    char sourceTargetDir = sourceJunc->getTarget()->getDir();
    bool shouldIncrease = sourceTargetDir==baseDir;
    // if extending direction is to left, the opposite situation
    if (extendingDir == 'l') {
        shouldIncrease = !shouldIncrease;
    }
    int isSourceLarger = 0 ;
    if (sinkJunc->getSource()->getId() < sourceJunc->getTarget()->getId()){
        isSourceLarger = 1;
    } else if (sinkJunc->getSource()->getId() > sourceJunc->getTarget()->getId()) {
        isSourceLarger = -1;
    }
    if ((shouldIncrease && isSourceLarger == 1) || (!shouldIncrease && isSourceLarger == -1))
        return false;
    return true;

}
bool FoldbackAlgorithm::isOppositeFoldbackJunction(Edge* sourceJunc,Edge* sinkJunc) {
    return sourceJunc->getTarget()->getId() == sinkJunc->getTarget()->getId() && sourceJunc->getSource()->getId() == sinkJunc->getSource()->getId();
}
vector<Vertex*> FoldbackAlgorithm::getFoldbackConnectionVertices(Edge* sourceJunc,Edge* sinkJunc) {
    vector<Vertex*> connectVertices;
    Vertex* currentVertex = sourceJunc->getTarget();
    Vertex* nextVertex;
    /*
     if the fold back source junction is the opposite to sink junction i.e A+ -> A- and A- -> A+
     noted that for fold back junction the complement junction is itself (A+ -> A-) is complementary to (A+ -> A-)
     */
    if (isOppositeFoldbackJunction(sourceJunc,sinkJunc)) {
        connectVertices.push_back(sinkJunc->getTarget());
        return connectVertices;
    }
    while (true) {
        Segment* nextSegment;
        char sourceTargetDir = sourceJunc->getTarget()->getDir();
        // if the source junction switch the string to strand same as base string
        if (sourceTargetDir==baseDir) {
            nextSegment = vertexOrderMap[currentVertex->getId()].second;
        } else {
            nextSegment = vertexOrderMap[currentVertex->getId()].first;
        }
        nextVertex = sourceTargetDir == '+' ? nextSegment->getPositiveVertex() : nextSegment->getNegativeVertex();
        connectVertices.push_back(nextVertex);
        currentVertex = nextVertex;
        // if reach the sink junction
        if (nextVertex->getId() == sinkJunc->getSource()->getId()) {
            connectVertices.push_back(sinkJunc->getTarget());
            return connectVertices;
        }
    }
}
