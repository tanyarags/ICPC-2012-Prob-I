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
struct PathNode{
    int x1, x2;
    int y1, y2;
    PathNode* next;
    
    PathNode(): next(nullptr){}
    PathNode(int x1, int y1, int x2, int y2): x1(x1), x2(x2), y1(y1), y2(y2), next(nullptr){}
    ~PathNode(){ delete next;}
};

struct Path{
    PathNode* hor_root;
    PathNode* ver_root;
    int len_hor, len_ver;
    
    Path():hor_root(nullptr), ver_root(nullptr), len_hor(0), len_ver(0){}
    
    bool push(int x1, int y1, int x2, int y2);
    bool _push(PathNode* node, PathNode* root);
    void print_path();
    
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
    
    bool add_neighbor(Base* neighbor){
        int direction = get_direction(neighbor);
        
        if (direction != -1){
            // changed to curr new node
            if( neighbor->type ==0) //Wall point
                directions[direction] = ((WallPoint*)neighbor)->closest_mirror;
            
            else
                directions[direction] = ((Mirror*)neighbor)->directions[direction];
            
            directions[(direction+2)%4] = neighbor; //curr node points to neighbor.
            
            //changed for neighbor
            if( neighbor->type ==0) //Wall point
                ((WallPoint*)neighbor)->closest_mirror = (Base*)this;
            else
                ((Mirror*)neighbor)->directions[direction] = (Base*)this;
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
    Base* start_node;
    Base* end_node;
    
    Maze(int row, int col) : row_tree(nullptr),col_tree(nullptr),
    rows(row),cols(col), start_node(nullptr), end_node(nullptr){
        row_tree = new AVLTree<WallNode>();
        col_tree = new AVLTree<WallNode>();
    };
    
    void init(){
        start_node = (Base*)(get_wall_node(col_tree, 0, 1 ));
        end_node = (Base*)(get_wall_node(col_tree, cols +1, rows));
    }
    
    bool add_mirror(int type, int x, int y);
    WallNode* get_wall_node(AVLTree<WallNode>* wall_tree, int x, int y);
    AVLTree<MirrorNode>* get_mirror_tree(WallNode* w_node);
    bool make_connections(Mirror* m, int key, int key_c, WallNode* w_node, AVLTree<MirrorNode>* m_tree);
    Path* traverse(Base* node, int direction);
    Base* next_node(Base* node, int direction);
    int emerging_direction(Base* node, int in_direction);
    
    Path* traverse_start(){
        return traverse(start_node, 1);
    }
    
    Path* traverse_end(){
        return traverse(end_node, 3);
    }
};
#endif /* maze_hpp */
