//
//  mirrors_grid.hpp
//  rapyuta
//
//  Created by tanyarags on 25/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#ifndef mirrors_grid_hpp
#define mirrors_grid_hpp

#include <stdio.h>

#include <iostream>
using namespace std;

struct Base{
    int type;
    int x;
    int y;
    Base(int type, int x, int y): type(type), x(x), y(y){}
};

struct Mirror: public Base {
    Base* north = nullptr; // Pointer to north mirror
    Base* south = nullptr; // Pointer to south mirror
    Base* east = nullptr; // Pointer to east mirror
    Base* west = nullptr; // Pointer to west mirror
    
    Mirror(int mirror_type, int x, int y): Base(mirror_type, x, y){}
};

struct SideNode: public Base {
    int key;
    int balance;
    Mirror *mirror;
    SideNode *left, *right, *parent;
    
    SideNode(int x, int y, Mirror* mirror, SideNode *p) : Base(0,x,y), balance(0), parent(p),
    left(nullptr), right(nullptr){
        if (x > 0) key = x;
        if (y > 0) key = y;
        this->mirror = mirror;
    }
    
    ~SideNode() {
        delete left;
        delete right;
        delete mirror;
    }
};

struct BalancedTree {
    
    int x_multiplier;
    int y_multiplier;
    SideNode *root;
    
    BalancedTree(bool is_row_tree);
    ~BalancedTree(void);
    SideNode* insert(Mirror *mirror);
    SideNode* search(Mirror *mirror, SideNode *start_node = nullptr);
    bool update(Mirror *mirror);
    void printBalance();
   
    SideNode* rotateLeft          ( SideNode *a );
    SideNode* rotateRight         ( SideNode *a );
    SideNode* rotateLeftThenRight ( SideNode *n );
    SideNode* rotateRightThenLeft ( SideNode *n );
    void rebalance                  ( SideNode *n );
    int height                      ( SideNode *n );
    void setBalance                 ( SideNode *n );
    void printBalance               ( SideNode *n );
    void clearNode                  ( SideNode *n );
    int get_key                     (Mirror *mirror);
};

#endif /* mirrors_grid_hpp */
