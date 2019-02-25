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
#include "avl_tree.cpp"

struct Mirror: public Base{
    Base *north;
    Base *south;
    Base *east;
    Base *west;
    
    Mirror(int type, int x, int y): Base(type,x,y){
        north = nullptr;
        south = nullptr;
        east = nullptr;
        west = nullptr;
    }
};

struct MirrorNode: public AVLNode{
    Mirror *mirror;
    
    MirrorNode(int key, int type, int x, int y, Mirror *mirror): AVLNode(key,type,x,y, nullptr){
        mirror = mirror;
    }
};

struct WallPoint: public Base{
    Base *less;
    Base *more;
    
    WallPoint(int type, int x, int y): Base(type,x,y){
        less = nullptr;
        more = nullptr;
    }
};


struct WallNode: public AVLNode{
    WallPoint *wallpoint;
    MirrorNode * mirror_root;
    
    WallNode(int key, int type, int x, int y): AVLNode(key,type,x,y, nullptr){
        wallpoint = new WallPoint(type, x, y);
        mirror_root = nullptr;
    }
    
    bool add_mirror_root( MirrorNode * mirror_root){
        this->mirror_root = mirror_root;
        return true;
    }
};


struct Maze {
    AVLTree<WallNode> *row_tree;
    AVLTree<WallNode> *col_tree;
    Maze() : row_tree(nullptr),col_tree(nullptr){};
    
    bool add_mirror(int type, int x, int y);
    
};
#endif /* maze_hpp */
