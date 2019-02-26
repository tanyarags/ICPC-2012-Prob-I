//
//  avl_tree.hpp
//  rapyuta
//
//  Created by tanyarags on 25/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#ifndef avl_tree_hpp
#define avl_tree_hpp

#include <stdio.h>
#include <iostream>
using namespace std;


struct AVLNode{
    int key;
    int balance;
    AVLNode *left, *right, *parent;
    
    AVLNode(int k, AVLNode *p) : key(k), balance(0), parent(p),
    left(nullptr), right(nullptr){}
    
    ~AVLNode() {
        delete left;
        delete right;
    }
};


template <class T>
struct AVLTree {
    
    T *root;
    AVLTree();
    ~AVLTree(void);
    T* insert(int key, T* add_node);
    T* search(int key, T *start_node = nullptr);
    T* search_less(int key, T *start_node = nullptr);
    T* search_more(int key, T *start_node = nullptr);
    void printBalance();
//    void printPoints();
    
    T* rotateLeft          ( T *a );
    T* rotateRight         ( T *a );
    T* rotateLeftThenRight ( T *n );
    T* rotateRightThenLeft ( T *n );
    void rebalance                  ( T *n );
    int height                      ( T *n );
    void setBalance                 ( T *n );
    void printBalance               ( T *n );
//    void printPoints                ( T *n );
    void clearNode                  ( T *n );
    T* leftmost                     ( T *n );
    T* rightmost                    ( T *n );
    T* next                         ( T *n );
};

#endif /* avl_tree_hpp */
