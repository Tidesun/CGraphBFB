#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <iomanip>

#include "Graph.hpp"
#include "Exceptions.hpp"

using namespace std;

Graph::Graph() {
    mPurity = -1;
    mAvgPloidy = -1;
    mAvgTumorPloidy = -1;

    mSegments = new vector<Segment *>();
    mJunctions = new vector<Junction *>();
}
Graph::Graph(const char * aFilename) {
    mPurity = -1;
    mAvgPloidy = -1;
    mAvgTumorPloidy = -1;

    mSource = NULL;
    mSink = NULL;

    mSegments = new vector<Segment *>();
    mJunctions = new vector<Junction *>();

    this->readGraph(aFilename);
}
Graph::~Graph() {;}

string Graph::getSampleName() { return mSampleName; }

int Graph::getJunctionIndexByEdge(Edge * aEdge) {
    int idx = 0;
    while (true) {
        if ((*mJunctions)[idx] == aEdge->getJunction())
            return idx;
        idx++;
    }
    throw JunctionDoesNotExistException(aEdge);
}

int Graph::getExpectedPloidy() { return mExpectedPloidy; }

double Graph::getPurity() { return mPurity; }
double Graph::getAvgPloidy() { return mAvgPloidy; }
double Graph::getAvgTumorPloidy() { return mAvgTumorPloidy; }
double Graph::getAvgCoverage() { return mAvgCoverage; }
double Graph::getAvgRawCoverage() { return mAvgCoverageRaw; }
void Graph::setPurity(double aPurity) { mPurity = aPurity; }
void Graph::setAvgPloidy(double aAvgPloidy) { mAvgPloidy = aAvgPloidy; }
void Graph::setAvgTumorPloidy(double aAvgTumorPloidy) { mAvgTumorPloidy = aAvgTumorPloidy; }
void Graph::setAvgCoverage(double aAvgCoverage) { mAvgCoverage = aAvgCoverage; }
void Graph::setAvgRawCoverage(double aAvgRawCoverage) { mAvgCoverageRaw = aAvgRawCoverage; }

Segment * Graph::getSource() { return mSource; }
Segment * Graph::getSink() { return mSink; }

vector<Segment *> * Graph::getSegments() { return mSegments; }
vector<Junction *> * Graph::getJunctions() { return mJunctions; }

// functionality
void Graph::readGraph(const char * aFilename) {
    ifstream graphFile(aFilename);
    if (!graphFile) {
        cerr << "Cannot open file " << aFilename << endl;
        exit(1);
    }
    
    cout << "Reading graph..." << endl;
    char line[8192];
    char * token;
    int sourceId, sinkId;
    while (!graphFile.eof()) {
        graphFile.getline(line, 8192);
        
        char * line_p = line;
        while (*line_p != '\0') {
            if (*line_p != '\t' && *line_p != ' ') {
                break;
            }
            line_p++;
        }
        if (*line_p == '#') {
            continue;
        }
        // cout << "\"" << line << "\"" << endl;

        token = strtok(line, " \t");
        if (token == NULL) {
            continue;
        }
        if (strcmp(token, "SAMPLE") == 0) {
            mSampleName = string(strtok(NULL, " "));
        } else if (strcmp(token, "AVG_DP") == 0) {
            mAvgCoverage = atof(strtok(NULL, " "));
            mAvgCoverageRaw = mAvgCoverage;
        } else if (strcmp(token, "PURITY") == 0) {
            mPurity = atof(strtok(NULL, " "));
        } else if (strcmp(token, "AVG_TUMOR_PLOIDY") == 0) {
            mAvgTumorPloidy = atof(strtok(NULL, " "));
        } else if (strcmp(token, "AVG_PLOIDY") == 0) {
            mAvgPloidy = atof(strtok(NULL, " "));
        } else if (strcmp(token, "PLOIDY") == 0) {
            token = strtok(NULL, " ");
            mPloidy = string(token);
            mExpectedPloidy = atoi(strtok(token, "m"));
        } else if (strcmp(token, "SOURCE") == 0) {
            token = strtok(NULL, " ");
            strtok(token, ":");
            sourceId = atoi(strtok(NULL, ":"));
        } else if (strcmp(token, "SINK") == 0) {
            token = strtok(NULL, " ");
            strtok(token, ":");
            sinkId = atoi(strtok(NULL, ":"));
        } else if (strcmp(token, "SEG") == 0) {
            char * node = strtok(NULL, " ");
            double segCoverage = max(atof(strtok(NULL, " ")), 0.0);

            strtok(node, ":");
            int segId = atoi(strtok(NULL, ":"));
            double segCred = 1.0;
            
            this->addSegment(segId, segCoverage, segCred);
        } else if (strcmp(token, "JUNC") == 0) {
            char * sourceNode = strtok(NULL, " ");
            char * targetNode = strtok(NULL, " ");
            double junCoverage = atof(strtok(NULL, " "));
            if (junCoverage <= 0) continue;
            
            strtok(sourceNode, ":");
            int sourceId = atoi(strtok(NULL, ":"));
            char sourceDir = strtok(NULL, ":")[0];

            strtok(targetNode, ":");
            int targetId = atoi(strtok(NULL, ":"));
            char targetDir = strtok(NULL, ":")[0];

            double junCred = 1.0;
            
            this->addJunction(sourceId, sourceDir, targetId, targetDir, junCoverage, junCred);
        }
    }
    mInferredBegin = mJunctions->end();
    
    mSource = this->getSegmentById(sourceId);
    mSink = this->getSegmentById(sinkId);
}

void Graph::writeGraph(const char * aFilename) {
    ofstream fout(aFilename);
    fout << "SAMPLE_NAME " << mSampleName << endl
         << "AVG_DP " << mAvgCoverage << endl
         << "PURITY " << mPurity << endl
         << "AVG_PLOIDY " << mAvgPloidy << endl
         << "PLOIDY " << mPloidy << endl
         << "SOURCE " << "H:" << mSource->getId() << endl
         << "SINK " << "H:" << mSink->getId() << endl;
    for (Segment * seg : *mSegments) {
        fout << "SEG " << "H:" << seg->getId() << " " << seg->getWeight()->getCoverage() << endl;
    }
    for (Junction * junc : *mJunctions) {
        Edge * e = junc->getEdgeA();
        fout << "JUNC " << "H:" << e->getSource()->getId() << ":" << e->getSource()->getDir() << " "
             << "H:" << e->getTarget()->getId() << ":" << e->getTarget()->getDir() << " " << junc->getWeight()->getCoverage() << endl;
    }
    fout.close();
}

void Graph::checkOrphan() {
    for (Segment * seg : *mSegments) {
        seg->checkOrphan();
    }
}

void Graph::calculateCopyNum() {
    /* haploid depth = average depth / average ploidy
     * average ploidy = purity * average tumor ploidy + (1 - purity) * 2
     * purity * segment copy * haploytye depth + (1 - purity) * 2 * haploid depth = segment depth
    */
    
    if (mAvgPloidy < 0) {
        if (mAvgTumorPloidy < 0) {
            cerr << "input error: there is no ploidy information provided. There must be at least one of \"AVG_PLOIDY\" and \"AVG_TUMOR_PLOIDY\"." << endl;
            exit(1);
        } else {
            if (mPurity < 0) {
                cerr << "input error: no purity information provided." << endl;
                exit(1);
            } else {
                mAvgPloidy = mPurity * mAvgTumorPloidy + (1 - mPurity) * 2;
            }
        }
    } else {
        if (mAvgTumorPloidy >= 0) {
            cout << "WARN: both AVG_PLOIDY and AVG_TUMOR_PLOIDY are provided, ";

            if (mPurity < 0) {
                cout << "WARN: no purity information provided, use the given AVG_PLOIDY" << endl;
            } else {
                double avgPloidy = mPurity * mAvgTumorPloidy + (1 - mPurity) * 2;
                if (abs(mAvgPloidy - avgPloidy) <= 0.1) {
                    cout << "calculated AVG_PLOIDY using AVG_TUMOR_PLOIDY is close enough to the given AVG_PLOIDY, use the given AVG_PLOIDY" << endl;
                } else {
                    mAvgPloidy = avgPloidy;
                    cout << "calculated AVG_PLOIDY using AVG_TUMOR_PLOIDY is distinguishable from the given AVG_PLOIDY, use the calculated AVG_PLOIDY" << endl;
                }
            }
        } else {
            cout << "WARN: only AVG_PLOIDY is given, use that" << endl;
        }
    }

    mHaploidDepth = mAvgCoverageRaw / mAvgPloidy;
    cout << "Average ploidy: " << mAvgPloidy << endl
        << "Haploid depth: " << mHaploidDepth << endl;
    mAvgCoverage = 2 * mHaploidDepth;

    for (Segment * seg : *mSegments) {
        double segCopy = (seg->getWeight()->getCoverage() - (1 - mPurity) * mAvgCoverage) / (mPurity * mHaploidDepth);
        // seg->getWeight()->setAdjustedCoverage(max(segAdjustedCoverage, 0.0));
        // seg->getWeight()->setCoverage(seg->getWeight()->getAdjustedCoverage());
        seg->getWeight()->setCopyNum(max(segCopy, 0.0));
    }

    for (Junction * junc : *mJunctions) {
        double juncCopy = junc->getWeight()->getCoverage() / mHaploidDepth;
        // junc->getWeight()->setAdjustedCoverage(max(juncAdjustedCoverage, 0.0));
        // junc->getWeight()->setCoverage(junc->getWeight()->getAdjustedCoverage());
        junc->getWeight()->setCopyNum(max(juncCopy, 0.0));
    }
}

void Graph::restoreCopy() {
    for (Segment * seg : *mSegments) {
        seg->restoreCopy();
    }
    for (Junction * junc : *mJunctions) {
        junc->restoreCopy();
    }
}

void Graph::backupCopy() {
    for (Segment * seg : *mSegments) {
        seg->backupCopy();
    }
    for (Junction * junc : *mJunctions) {
        junc->backupCopy();
    }
}

void Graph::resetVertexVisitFlag() {
    for (Segment * seg : *mSegments) {
        seg->getPositiveVertex()->resetVisited();
        seg->getNegativeVertex()->resetVisited();
        // seg->getPositiveVertex()->clearShortestPrevVertex();
        // seg->getNegativeVertex()->clearShortestPrevVertex();
        // seg->getPositiveVertex()->clearShortestPrevEdge();
        // seg->getNegativeVertex()->clearShortestPrevEdge();
    }
}

void Graph::resetJunctionVisitFlag() {
    for (Junction * junc : *mJunctions) {
        junc->getEdgeA()->resetVisited();
        junc->getEdgeB()->resetVisited();
    }
}

void Graph::checkLowerBound() {
    for (Segment * seg : *mSegments) {
        seg->checkLowerBound();
    }
    for (Junction * junc : *mJunctions) {
        junc->checkLowerBound();
    }
}

bool Graph::isCopyExhaustive() {
    for (Segment * seg : *mSegments) {
        if (seg->hasCopy()) {
            return false;
        }
    }
    return true;
}

bool Graph::doesJunctionExist(Junction * aJunction) {
    vector<string> aJuncInfo = aJunction->getInfo();
    for (Junction * junc : *mJunctions) {
        vector<string> juncInfo = junc->getInfo();
        if ((juncInfo[0] == aJuncInfo[0] && juncInfo[1] == aJuncInfo[1]) || (juncInfo[0] == aJuncInfo[1] && juncInfo[1] == aJuncInfo[0])) {
            return true;
        }
    }
    return false;
}

Segment * Graph::getSegmentById(int aSegId) {
    for (Segment * seg : *mSegments) {
        if (seg->getId() == aSegId) {
            return seg;
        }
    }
    throw SegmentDoesNotExistException(aSegId);
}

Segment * Graph::addSegment(int aId, double aCoverage, double aCredibility) {
    Segment * seg = new Segment(aId, aCoverage, aCredibility);
    mSegments->push_back(seg);
    return seg;
}

Junction * Graph::addJunction(Vertex * aSource, Vertex * aTarget, double aCoverage, double aCredibility, bool aInferred, bool aIsSourceSinkJunction) {
    Segment * sourceSeg = aSource->getSegment();
    Segment * targetSeg = aTarget->getSegment();

    Junction * junc = new Junction(sourceSeg, targetSeg, aSource->getDir(), aTarget->getDir(), aCoverage, aCredibility, aInferred, aIsSourceSinkJunction);
    if (this->doesJunctionExist(junc)) {
        throw DuplicateJunctionException(junc);
    }
    junc->insertEdgesToVertices();
    mJunctions->push_back(junc);
    // if (aIsSourceSinkJunction) {
    //     mJunctions->push_back(junc);
    // } else {
    //     if (mJunctions->size() > 0) {
    //         mJunctions->insert(mJunctions->end() - 1, junc);
    //     } else {
    //         mJunctions->push_back(junc);
    //     }
    // }
    return junc;
    // TODO
    // if mInferred, append to a vector storing inferred junctions
}

Junction * Graph::addJunction(int aSourceId, char aSourceDir, int aTargetId, char aTargetDir, double aCoverage, double aCredibility, bool aInferred, bool aIsSourceSinkJunction) {
    Segment * sourceSeg = this->getSegmentById(aSourceId);
    Segment * targetSeg = this->getSegmentById(aTargetId);

    Junction * junc = new Junction(sourceSeg, targetSeg, aSourceDir, aTargetDir, aCoverage, aCredibility, aInferred, aIsSourceSinkJunction);
    if (this->doesJunctionExist(junc)) {
        throw DuplicateJunctionException(junc);
    }
    junc->insertEdgesToVertices();
    mJunctions->push_back(junc);
    // if (aIsSourceSinkJunction) {
    //     mJunctions->push_back(junc);
    // } else {
    //     if (mJunctions->size() > 0) {
    //         mJunctions->insert(mJunctions->end() - 1, junc);
    //     } else {
    //         mJunctions->push_back(junc);
    //     }
    // }
    return junc;
    // TODO
    // if mInferred, append to a vector storing inferred junctions
}

Vertex * Graph::getNextVertexById(Vertex * aSourceVertex) {
    Segment * nextSegment;
    if (aSourceVertex->getDir() == '+') {
        nextSegment = this->getSegmentById(aSourceVertex->getId() + 1);
        // if (nextSegment == NULL) {
        //     throw NotReachableException(aSourceVertex);
        // }
        return nextSegment->getPositiveVertex();
    } else {
        nextSegment = this->getSegmentById(aSourceVertex->getId() - 1);
        // if (nextSegment == NULL) {
        //     throw NotReachableException(aSourceVertex);
        // }
        return nextSegment->getNegativeVertex();
    }
    // return nextSegment->getNegativeVertex();
}

Vertex * Graph::getPrevVertexById(Vertex * aTargetVertex) {
    Segment * prevSegment;
    if (aTargetVertex->getDir() == '+') {
        prevSegment = this->getSegmentById(aTargetVertex->getId() - 1);
        // if (prevSegment == NULL) {
        //     throw NotReachableException(aTargetVertex);
        // }
        return prevSegment->getPositiveVertex();
    } else {
        prevSegment = this->getSegmentById(aTargetVertex->getId() + 1);
        // if (prevSegment == NULL) {
        //     throw NotReachableException(aTargetVertex);
        // }
        return prevSegment->getNegativeVertex();
    }
    // return prevSegment->getNegativeVertex();
}

void Graph::print() {
    cout << "``````````````````````````````````````````````````````````````````````````````````````````" << endl;
    cout << "Ploidy: " << mExpectedPloidy << endl;
    cout << "Avg ploidy: " << mAvgPloidy << endl;
    cout << "Avg coverage: " << mAvgCoverage << endl;
    cout << "Haploid coverage: " << mHaploidDepth << endl;
    cout << "Source: " << to_string(mSource->getId()) << endl;
    cout << "Sink: " << to_string(mSink->getId()) << endl;
    cout << "Segments: " << mSegments->size() << endl;
    cout << "Junctions: " << mJunctions->size() << endl;
    cout << "``````````````````````````````````````````````````````````````````````````````````````````" << endl;
    for (Segment * seg : *mSegments) {
        cout << fixed << setprecision(4)
            << "SEG" << "\t"
            << seg->getId() << "\t" 
            << seg->getWeight()->getCoverage() << "\t"
            // << seg->getWeight()->getOriginalCoverage() << "\t" 
            // << seg->getWeight()->getAdjustedCoverage() << "\t" 
            << "\033[1;31m" << seg->getWeight()->getCopyNum() << "\033[0m" << "\t" 
            << seg->getCredibility() << "\t" 
            << (seg->isOrphan() ? "OO" : "ONO") << "\t" 
            << (seg->getPositiveVertex()->isOrphan() ? "OO" : "ONO") << "\t"
            << (seg->getNegativeVertex()->isOrphan() ? "OO" : "ONO") << "\t"
            << (seg->isOrphan(false) ? "O" : "NO") << "\t" 
            << (seg->getPositiveVertex()->isOrphan(false) ? "O" : "NO") << "\t"
            << (seg->getNegativeVertex()->isOrphan(false) ? "O" : "NO") << "\t"
            << (seg->isDeadEnd() ? "D" : "ND") << "\t"
            << (seg->hasLowerBoundLimit() ? "LB" : "NLB") << endl;
    }

    int c = 0;
    for (Junction * junc : *mJunctions) {
        vector<string> info = junc->getInfo();
        cout << left << fixed << setprecision(4)
            << "JUNC" << "\t"
            << c << "\t"
            << info[0] << "\t" << info[1] << "\t"
            << junc->getWeight()->getCoverage() << "\t"
            // << junc->getWeight()->getOriginalCoverage() << "\t"
            // << junc->getWeight()->getAdjustedCoverage() << "\t"
            << "\033[1;31m" << junc->getWeight()->getCopyNum() << "\033[0m" << "\t"
            << junc->getCredibility() << "\t"
            << (junc->isInferred() ? "I" : "NI") << "\t"
            << (junc->hasLowerBoundLimit() ? "LB" : "NLB") << endl;
        c++;
    }
    cout << "``````````````````````````````````````````````````````````````````````````````````````````" << endl;
}
