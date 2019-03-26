#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>

#include "Segment.hpp"
#include "Junction.hpp"

using namespace std;

class Segment;
class Junction;

class Graph {
    protected:
        string mSampleName;
        
        int mExpectedPloidy;     // expected ploidy

        double mHaploidDepth;
        double mPurity;   // tumor purity
        double mAvgPloidy;   // the ploidy of whole sequencing region, including tumor if possible
        double mAvgTumorPloidy;   // the ploidy of tumor
        double mAvgCoverage;
        double mAvgCoverageRaw;

        string mPloidy;

        Segment * mSource;
        Segment * mSink;

        vector<Segment *> * mSegments;
        vector<Junction *> * mJunctions;
        vector<Junction *>::iterator mInferredBegin;

    public:
        Graph();
        Graph(const char * aFilename);
        ~Graph();

        string getSampleName();

        int getJunctionIndexByEdge(Edge * aEdge);

        int getExpectedPloidy();
        double getPurity();
        double getAvgPloidy();
        double getAvgTumorPloidy();
        double getAvgCoverage();
        double getAvgRawCoverage();
        void setPurity(double aPurity);
        void setAvgPloidy(double aAvgPloidy);
        void setAvgTumorPloidy(double aAvgTumorPloidy);
        void setAvgCoverage(double aAvgCoverage);
        void setAvgRawCoverage(double aAvgCoverageRaw);

        Segment * getSource();
        Segment * getSink();
        
        vector<Segment *> * getSegments();
        vector<Junction *> * getJunctions();

        /* functionality */
        
        bool isCopyExhaustive();
        bool doesJunctionExist(Junction * aJunction);

        void readGraph(const char * aFilename);
        void writeGraph(const char * aFilename);
        void checkOrphan();
        void calculateCopyNum();
        void restoreCopy();
        void backupCopy();
        void resetVertexVisitFlag();
        void resetJunctionVisitFlag();
        void checkLowerBound();

        Segment * getSegmentById(int aSegId);
        Segment * addSegment(int aId, double aCoverage, double aCredibility);
        Junction * addJunction(Vertex * aSource, Vertex * aTarget, double aCoverage, double aCredibility, bool aInferred = false, bool aIsSourceSinkJunction = false);
        Junction * addJunction(int aSourceId, char aSourceDir, int aTargetId, char aTargetDir, double aCoverage, double aCredibility, bool aInferred = false, bool aIsSourceSinkJunction = false);
        Vertex * getPrevVertexById(Vertex * aTargetVertex);
        Vertex * getNextVertexById(Vertex * aSourceVertex);

        void print();
};

#endif
