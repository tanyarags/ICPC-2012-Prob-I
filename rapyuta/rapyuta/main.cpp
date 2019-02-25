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
    
    Maze maze(r,c);
    maze.init();
    
    int tmp_x = -1, tmp_y= -1;
    for (int iter = 0; iter < m; iter++)
    {
        cin >> tmp_y >> tmp_x;
        maze.add_mirror(-1, tmp_x, tmp_y);
    }
    
    for (int iter = 0; iter < n; iter++)
    {
        cin >> tmp_y >> tmp_x;
        maze.add_mirror(1, tmp_x, tmp_y);
    }
    
    Path* path1 = maze.traverse_start();
    Path* path2 = maze.traverse_end();
    
    path1->print_path();
    path2->print_path();
    
    
    return 0;
}
