//
// Created by Tidesun on 2019-04-23.
//

#ifndef BFBGRAPH_FILE_READER_H
#define BFBGRAPH_FILE_READER_H
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>
#include <map>
#include "Graph.hpp"
using namespace std;
class FileReader {
private:
    string fileName;
    char delimeter;
    int format;
public:
    enum {
        SVCN, GRAPH
    };
    FileReader(string _fileName,int _format);
    vector<vector<string> > getData();
    void readGraph(Graph* g);
    void convertReadGraph(Graph* g);
};
#endif //BFBGRAPH_FILE_READER_H
