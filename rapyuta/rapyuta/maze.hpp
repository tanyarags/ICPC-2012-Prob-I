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

struct LineSegment{
    int x1, x2, y1, y2;
    LineSegment(int x1, int y1, int x2, int y2): x1(x1), y1(y1), x2(x2), y2(y2){};
    void print(){
        cout<< "\nPoint1 (" << x1 << "," << y1 << ") Point2 (" << x2 << ","<<y2 << ")";
    }
};

struct PathNode: public LineSegment{
    int key;
    int balance;
    
    PathNode *left, *right, *parent;
    
    PathNode(int k, int x1, int y1, int x2, int y2, PathNode* p): LineSegment(x1, y1, x2, y2), key(k), balance(0), parent(p),
    left(nullptr), right(nullptr){};
    
    ~PathNode() {
        delete left;
        delete right;
    }
};

struct Path{
    AVLTree<PathNode> *hor_root;
    AVLTree<PathNode> *ver_root;
    int len_hor, len_ver;
    
    Path():hor_root(nullptr), ver_root(nullptr), len_hor(0), len_ver(0){
        hor_root = new AVLTree<PathNode>();
        ver_root = new AVLTree<PathNode>();
    };
    
    bool push(int x1, int y1, int x2, int y2);
    void print_path();
    void print_tree(PathNode* node);
    
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

struct MirrorNode: public AVLNode{
    Mirror *mirror;
    
    MirrorNode(int key, int type, int x, int y, Mirror *mirror): AVLNode(key,type,x,y, nullptr){
        this->mirror = mirror;
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
    Path* traverse(Base* node, int direction);
    Base* next_node(Base* node, int direction);
    int emerging_direction(Base* node, int in_direction);
    bool make_connection_last_layer(Mirror* m, int key, int key_c, WallNode* w_node);
    
    Path* traverse_start(){
        return traverse((Base*)(start_node->wallpoint), 1);
    }
    
    Path* traverse_end(){
        return traverse((Base*)(end_node->wallpoint), 3);
    }
};
#endif /* maze_hpp */
