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

struct Base{
    int type;
    int x;
    int y;
    Base(int type, int x, int y): type(type), x(x), y(y){}
};

struct GridNode: public Base, public AVLNode {
    GridNode(int k, int type, int x, int y, GridNode *p) : Base(type,x,y), AVLNode(k,(AVLNode*)p){}
    ~GridNode() {}
};

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
    
    bool add_neighbor(Base* neighbor);
    int get_direction(Base* node);
};

struct MirrorNode: public GridNode{
    Mirror *mirror;
    
    MirrorNode(int key, int type, int x, int y, Mirror *mirror): GridNode(key,type,x,y, nullptr){
        this->mirror = mirror;
    }
};

struct WallNode: public GridNode{
    WallPoint *wallpoint;
    AVLTree<MirrorNode> *mirror_tree;
    
    WallNode(int key, int type, int x, int y): GridNode(key,type,x,y, nullptr){
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
    int rows;
    int cols;
    WallNode* start_node;
    WallNode* end_node;
    bool added_last_row = false;
    
    Maze(int row, int col) : row_tree(nullptr),col_tree(nullptr),
    rows(row),cols(col), start_node(nullptr), end_node(nullptr){
        row_tree = new AVLTree<WallNode>();
        col_tree = new AVLTree<WallNode>();
    };
    
    void init(){
        start_node = get_wall_node(col_tree, 0, 1 );
        end_node = new WallNode(cols + 1 + rows, 0, cols +1, rows);
    }
    
    bool add_mirror(int type, int x, int y);
    WallNode* get_wall_node(AVLTree<WallNode>* wall_tree, int x, int y);
    AVLTree<MirrorNode>* get_mirror_tree(WallNode* w_node);
    bool make_connections(Mirror* m, int key, int key_c, WallNode* w_node, AVLTree<MirrorNode>* m_tree);
    bool make_connection_last_layer(Mirror* m, int key, int key_c, WallNode* w_node);
    
    
};
#endif /* maze_hpp */
