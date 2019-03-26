//
//  main.cpp
//  BFBDetectGraph
//
//  Created by Tidesun on 18/3/2019.
//  Copyright © 2019 Oliver. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
vector<int> findPrefixMaximalPalindrome(string s);
vector<int> RtoP(vector<int> R);
bool checkBFB(string x,string x0);
vector<int> findPrefixMaximalPalindrome(string s){
    int i = 0;
    int l = 0;
    vector<int> R(0);
    while (i + l < s.length()-1) {
        while (i-l>=0 && i + l < s.length()-1 && s[i-l]==s[i+l+1]){
            l++;
        }
        R.push_back(l);
        int c = 0;
        bool isFoundSuffix = false;
        for (int j=0;j<l-1;j++){
            if (l-j-1<=R[i-j-1]){
                l = l-j-1;
                isFoundSuffix = true;
                break;
            }
            R.push_back(min(l-j-1,R[i-j-1]));
            c++;
        }
        if (!isFoundSuffix){
            l = 0;
        }
        i += c + 1;
    }
    return RtoP(R);
}
vector<int> RtoP(vector<int> R){
    vector<int> P(R.size());
    for (int i=0;i<R.size()-1;i++){
        P[i-R[i]+1] = max(P[i-R[i]+1],R[i]);
    }
    int palCounter = 0;
    for (int i=0;i<P.size();i++){
        palCounter = max(P[i],palCounter-1)>0 ? max(P[i],palCounter-1):0;
        P[i] = max(P[i],palCounter);
    }
    return P;
}
string createBaseStr(int n) {
    string s;
    for (int i=0;i<n;i++){
        s += 'A'+i;
    }
    return s;
}
bool checkBFB(string x,string x0){
    // check whether x0 is a prefix of x
    if (x.compare(x.length()-x0.length(),x0.length(),x0)!=0){
        return false;
    }
    vector<int> P = findPrefixMaximalPalindrome(x);
    int i = 0;
    while (i<x.length()-x0.length()){
        if (P[i]==0) return false;
        i += P[i];
    }
    return true;
}
bool BFBPivot(vector<int> n,map<pair<int,int>,int> breakpoints,string s, vector<int> n0){
    // n is target count vector, n0 is count vector of s
    if (!checkBFB(s,createBaseStr((int)n.size()))){
        return false;
    }
    if (n0==n){
        cout << s;
        return true;
    }
    // use lexical order to represent a character
    int order_firstChar = s[0]-'A';
    vector<int> order_chars = {order_firstChar};
    if (n0[order_firstChar] % 2 == 0){
        int order_nextChar = order_firstChar+1;
        if (order_nextChar<n.size())
            order_chars.push_back(order_nextChar);
    } else {
        int order_nextChar = order_firstChar - 1;
        if (order_nextChar>=0)
            order_chars.push_back(order_nextChar);
    }
    for (int i=0;i<order_chars.size();i++){
        char _char = 'A'+order_chars[i];
        int order_char = order_chars[i];
        if (n0[order_char] != n[order_char] && breakpoints[make_pair(order_char,order_firstChar)]>0){
            vector<int> n1(n0);
            n1[order_chars[i]] ++;
            breakpoints[make_pair(order_char,order_firstChar)]--;
            if (BFBPivot(n,breakpoints,_char+s,n1))
                return true;
            breakpoints[make_pair(order_char,order_firstChar)]++;
        }
    }
    return false;
}
//int main(int argc, const char * argv[]) {
//    vector<int> n = {14,7,19,17,9,13};
//    vector<int> n = {10,4,5,9,7,3,3,3};
//    map<pair<int,int>,int> breakpoints;
//    for (int i=0;i<n.size();i++)
//        for (int j=0;j<n.size();j++)
//            breakpoints.insert(make_pair(make_pair(i,j),1));
//
//    //vector<int> n = {6,4,3};
//    cout << checkBFB("AAABC","ABC");
//    cout<<BFBPivot(n,breakpoints,createBaseStr((int)n.size()),vector<int>((int)n.size(),1));
//    vector<int> n = findPrefixMaximalPalindrome("BAABCCBAAAABC");
//    for (int i=0;i<n.size();i++){
//        cout<<n[i]<<' ';
//    }
//    cout<<endl;
//    n = findPrefixMaximalPalindrome("AABCCBAAAABC");
//    for (int i=0;i<n.size();i++){
//        cout<<n[i]<<' ';
//    }
//    return 0;
//}
