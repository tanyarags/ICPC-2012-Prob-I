//
//  maze.cpp
//  rapyuta
//
//  Created by tanyarags on 25/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#include "maze.hpp"

bool Maze::add_mirror(int type, int x, int y){
   //create a mirror
    Mirror* m = new Mirror(type,x,y);
    
    
    //add mirror to row and column trees
    SideNode* side_row = row_tree->insert(m);
    SideNode* side_col = col_tree->insert(m);
    
    
    
    
    return true;
}
