//
//  main.cpp
//  rapyuta
//
//  Created by tanyarags on 24/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "maze.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    
    ifstream in("input.txt");
    cin.rdbuf(in.rdbuf()); //redirect
    int r, c, m, n;
    cin >> r >> c >> m >> n;
    
    int tmp_x = -1, tmp_y= -1;
    for (int iter = 0; iter < m; iter++)
    {
        cin >> tmp_x >> tmp_y;
    }
    
    for (int iter = 0; iter < n; iter++)
    {
        cin >> tmp_x >> tmp_y;
    }
    
    
    return 0;
}
