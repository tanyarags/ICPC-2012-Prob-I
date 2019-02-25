//
//  maze.cpp
//  rapyuta
//
//  Created by tanyarags on 25/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#include "maze.hpp"

bool Maze::add_mirror(int type, int x, int y){
    int key_x = x;
    int key_y = y;
    
    //create a mirror
    Mirror* m = new Mirror(type,x,y);//new mirror
    
    //add wall node
    WallNode* node_x = row_tree->search(key_x);
    if (node_x == nullptr){
        //wall node doesnt exist, add new
        node_x = new WallNode(key_x, 0, x, y);
        node_x = row_tree->insert(key_x, node_x);
    }
    else{
       // if node_x
    }
    
    WallNode* node_y = new WallNode(key_y, 0, x, y);
    node_y = row_tree->insert(key_y, node_y);
    
    
    
    
    return true;
}
