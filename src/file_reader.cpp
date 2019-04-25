//
// Created by Tidesun on 2019-04-23.
//

#include "file_reader.hpp"
// util function to split string into vector
template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}
FileReader::FileReader(string _fileName,int _format) {
    fileName = _fileName;
    format = _format;
    delimeter = ',';
}
void FileReader::readGraph(Graph* g) {
    if (format == SVCN) {
        convertReadGraph(g);
    }
    if (format == GRAPH) {
        g->readGraph(fileName.c_str());
    }
    g->calculateCopyNum();
}
vector<vector<string> > FileReader::getData(){
    ifstream file(fileName);

    vector<std::vector<string> > dataList;

    string line = "";
    getline(file, line);
    vector<string> header = split(line,delimeter);
    // Iterate through each line and split the content using delimeter
    while (getline(file, line))
    {
        vector<string> vec = split(line,delimeter);
        dataList.push_back(vec);
    }
    // Close the File
    file.close();

    return dataList;
}
void FileReader::convertReadGraph(Graph* g) {
    vector<vector<string>> dataframe = getData();
    g->setAvgCoverage(20);
    g->setAvgRawCoverage(20);
    g->setAvgPloidy(2.0);
    g->setPurity(1.0);
    vector<pair<int,int>> breakpoints;
    vector<tuple<int,int,int,int>> segments;
    int bk_max = -1;
    int bk_min = numeric_limits<int>::max();
    for (auto row:dataframe) {
        int bkpos_5p = stoi(row[1]);
        int bkpos_3p = stoi(row[4]);
        int cn = stoi(row[6]);
        bk_max = max(bk_max,max(bkpos_3p,bkpos_5p));
        bk_min = min(bk_min,min(bkpos_3p,bkpos_5p));
        breakpoints.emplace_back(make_pair(bkpos_5p,cn));
        breakpoints.emplace_back(make_pair(bkpos_3p,cn));
    }
    breakpoints.push_back(make_pair(bk_max+1,10));
    breakpoints.push_back(make_pair(bk_min-1,10));
    sort(breakpoints.begin(), breakpoints.end());
    int index = 0;
    for (auto it = breakpoints.begin(),end = breakpoints.end();it != end-1;++it) {
        int left_bkpos = it->first;
        int right_bkpos = next(it,1)->first;
        int cn = max(it->second,next(it,1)->second);
        if (index >0 ) {
            int prev_cn = get<3>(segments.back());
            if (abs(cn - prev_cn) < 10) {
                get<2>(segments.back()) = right_bkpos;
            } else {
                segments.emplace_back(make_tuple(index,left_bkpos,right_bkpos,cn));
                index++;
            }
        } else {
            segments.emplace_back(make_tuple(index,left_bkpos,right_bkpos,cn));
            index ++;
        }
    }
    map<int, int> leftDic;
    map<int, int> rightDic;
    for (auto seg:segments){
        leftDic[get<1>(seg)] = get<0>(seg);
        rightDic[get<2>(seg)] = get<0>(seg);
        g->addSegment(get<0>(seg),get<3>(seg)+10,1.0);
    }
    for (auto it = segments.begin(),end = segments.end();it != end-1;++it) {
        int currentId = get<0>(*it);
        int nextId = get<0>(*next(it,1));
        g->addJunction(currentId,'+',nextId,'+',10,1.0);
    }
    for (auto row:dataframe) {
        int bkpos_5p = stoi(row[1]);
        int bkpos_3p = stoi(row[4]);
        char dir_5p = row[2][0];
        char dir_3p = row[5][0];
        int cn = stoi(row[6]);
        auto leftId = rightDic.upper_bound(bkpos_5p)->second;
        auto rightId = leftDic.upper_bound(bkpos_3p)->second;
        cout<< leftId << dir_5p << rightId<<dir_3p<<endl;
        g->addJunction(leftId,dir_5p,rightId,dir_3p,cn,1.0);
    }
}
