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

struct WallPoint: public Base{
    Base *closest_mirror;
    
    WallPoint(int type, int x, int y): Base(type,x,y){
        closest_mirror = nullptr;
    }
    bool add_neighbor(Base* neighbor){
        closest_mirror = neighbor;
        return true;
    }
};

struct Mirror: public Base{
    Base *directions[4]; // 0, 1, 2, 3: north, east south, west
    
    Mirror(int type, int x, int y): Base(type,x,y){
        for (int iter = 0; iter < 4; iter++)
            directions[iter] = nullptr;
    }
    
    bool add_neighbor(Base* neighbor){
        int direction = get_direction(neighbor);
        
        if (direction != -1){
            if( neighbor->type ==0) //Wall point
                directions[direction] = ((WallPoint*)neighbor)->closest_mirror;
            
            else
                directions[direction] = ((Mirror*)neighbor)->directions[direction];
            
            directions[(direction-2)%4] = neighbor;
        }
        return true;
    }
    
    int get_direction(Base* node){
        int ref_x = node->x, ref_y = node->y;
        
        // 0, 1, 2, 3: north, east south, west
        int direction = -1;
        
        if (ref_x == x)
            direction = y < ref_y? 0: 2 ;//north, south
        
        if(ref_y == y)
            direction = x < ref_x? 3: 1 ;//west, east
        
        return direction;
    }
};

struct MirrorNode: public AVLNode{
    Mirror *mirror;
    
    MirrorNode(int key, int type, int x, int y, Mirror *mirror): AVLNode(key,type,x,y, nullptr){
        mirror = mirror;
    }
};

struct WallNode: public AVLNode{
    WallPoint *wallpoint;
    AVLTree<MirrorNode> *mirror_tree;
    
    WallNode(int key, int type, int x, int y): AVLNode(key,type,x,y, nullptr){
        wallpoint = new WallPoint(type, x, y);
        mirror_tree = nullptr;
    }
    
    bool add_mirror_root( AVLTree<MirrorNode> * mirror_tree){
        this->mirror_tree = mirror_tree;
        return true;
    }
};


struct Maze {
    AVLTree<WallNode> *row_tree;
    AVLTree<WallNode> *col_tree;
    Maze() : row_tree(nullptr),col_tree(nullptr){};
    
    bool add_mirror(int type, int x, int y);
    WallNode* get_wall_node(Mirror *m, AVLTree<WallNode>* wall_tree, int key, int key_c);
    AVLTree<MirrorNode>* get_mirror_tree(Mirror *m, WallNode* w_node, int key, int key_c);
    bool make_connections(Mirror* m, int key, int key_c, WallNode* w_node, AVLTree<MirrorNode>* m_tree);
    //MirrorNode* get_mirror_node(Mirror *m, WallNode* w_node, int key, int key_c);
    
};
#endif /* maze_hpp */
