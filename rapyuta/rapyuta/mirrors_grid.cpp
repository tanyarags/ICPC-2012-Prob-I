//
//  mirrors_grid.cpp
//  rapyuta
//
//  Created by tanyarags on 25/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#include "mirrors_grid.hpp"

SideNode* BalancedTree::search(Mirror *mirror, SideNode *start_node){
    
    if (root == nullptr)
        return nullptr;
    else{
        if (start_node == nullptr)
            start_node = root;
        
        int key = get_key(mirror);
    
        if (start_node->key == key)
            return start_node;
        
        else{
            SideNode *n = start_node;
            
            bool goLeft = n->key > key;
            n = goLeft ? n->left : n->right;
            
            if (n == nullptr)
                return nullptr;
            
            else
                return search(mirror, n);
        }
    }
}

int BalancedTree::get_key(Mirror *mirror){
    int key = (mirror->x)* x_multiplier;
    if (key ==0)
        key = (mirror->y)* y_multiplier;
    return key;
}

bool BalancedTree::update(Mirror *mirror){
    SideNode* node = search(mirror);
    
    if ( node != nullptr){
        node->mirror = mirror;
    }
    
    return true;
}

void BalancedTree::rebalance(SideNode *n) {
    setBalance(n);

    if (n->balance == -2) {
        if (height(n->left->left) >= height(n->left->right))
            n = rotateRight(n);
        else
            n = rotateLeftThenRight(n);
    }
    else if (n->balance == 2) {
        if (height(n->right->right) >= height(n->right->left))
            n = rotateLeft(n);
        else
            n = rotateRightThenLeft(n);
    }

    if (n->parent != nullptr) {
        rebalance(n->parent);
    }
    else {
        root = n;
    }
}

SideNode* BalancedTree::rotateLeft(SideNode *a) {
    SideNode *b = a->right;
    b->parent = a->parent;
    a->right = b->left;

    if (a->right != nullptr)
        a->right->parent = a;

    b->left = a;
    a->parent = b;

    if (b->parent != nullptr) {
        if (b->parent->right == a) {
            b->parent->right = b;
        }
        else {
            b->parent->left = b;
        }
    }

    setBalance(a);
    setBalance(b);
    return b;
}

SideNode* BalancedTree::rotateRight(SideNode *a) {
    SideNode *b = a->left;
    b->parent = a->parent;
    a->left = b->right;

    if (a->left != nullptr)
        a->left->parent = a;

    b->right = a;
    a->parent = b;

    if (b->parent != nullptr) {
        if (b->parent->right == a) {
            b->parent->right = b;
        }
        else {
            b->parent->left = b;
        }
    }

    setBalance(a);
    setBalance(b);
    return b;
}

SideNode* BalancedTree::rotateLeftThenRight(SideNode *n) {
    n->left = rotateLeft(n->left);
    return rotateRight(n);
}

SideNode* BalancedTree::rotateRightThenLeft(SideNode *n) {
    n->right = rotateRight(n->right);
    return rotateLeft(n);
}

int BalancedTree::height(SideNode *n) {
    if (n == nullptr)
        return -1;
    return 1 + std::max(height(n->left), height(n->right));
}

void BalancedTree::setBalance(SideNode *n) {
    n->balance = height(n->right) - height(n->left);
}

void BalancedTree::printBalance(SideNode *n) {
    if (n != nullptr) {
        printBalance(n->left);
        std::cout << n->balance << " ";
        printBalance(n->right);
    }
}

BalancedTree::BalancedTree(bool is_row_tree) : root(nullptr) {
    x_multiplier = 0;
    y_multiplier = 0;

    if (is_row_tree == true) x_multiplier = 1;
    else y_multiplier = 1;
}

BalancedTree::~BalancedTree(void) {
    delete root;
}

SideNode* BalancedTree::insert(Mirror* mirror) {
    bool is_row = x_multiplier > 0;
    int key = is_row ? mirror->x : mirror->y;

    if (root == nullptr) {
        root = new SideNode(x_multiplier*mirror->x,
                            y_multiplier*mirror->y,
                            mirror,
                            nullptr); //(int x, int y, Mirror* mirror, SideNode *p)
        return root;
    }
    else {
        SideNode *n = root, *parent;
        SideNode* added_node;

        while (true) {
            if (n->key == key)
                return n;

            parent = n;

            bool goLeft = n->key > key;
            n = goLeft ? n->left : n->right;

            
            if (n == nullptr) {
                if (goLeft)
                    parent->left = added_node = new SideNode(x_multiplier*mirror->x,
                                                             y_multiplier*mirror->y,
                                                             mirror,
                                                             parent);
                
                else
                    parent->right = added_node = new SideNode(x_multiplier*mirror->x,
                                                              y_multiplier*mirror->y,
                                                              mirror,
                                                              parent);
                
                rebalance(parent);
                break;
            }
        }
        return added_node;
    }
}

void BalancedTree::printBalance() {
    printBalance(root);
    std::cout << std::endl;
}
