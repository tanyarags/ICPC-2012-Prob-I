//
//  maze.hpp
//  rapyuta
//
//  Created by tanyarags on 25/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#ifndef maze_hpp
#define maze_hpp

#include <stdio.h>
#include "mirrors_grid.hpp"

struct Maze {
    BalancedTree *row_tree;
    BalancedTree *col_tree;
    Maze() : row_tree(nullptr),col_tree(nullptr){};
    
    bool add_mirror(int type, int x, int y);
    
};
#endif /* maze_hpp */
