//
//  main.cpp
//  BFBDetectGraph
//
//  Created by Tidesun on 21/3/2019.
//  Copyright © 2019 Oliver. All rights reserved.
//

#include <iostream>
#include "bfb_algorithm.hpp"
#include "httpserver.cpp"
using namespace std;

int main() {
    httpServer().run_server("0.0.0.0","8080");
    return 0;
}
