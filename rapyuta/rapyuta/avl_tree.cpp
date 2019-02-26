//
//  avl_tree.cpp
//  rapyuta
//
//  Created by tanyarags on 25/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#include "avl_tree.hpp"

template <class T>
T* AVLTree<T>::leftmost(T* n){
    if (n == nullptr)
        n = root;
    
    if (n == nullptr)
        return nullptr;
    
    else{
        if(n->left == nullptr)
            return n;
        else
            return leftmost(n->left);
        
    }
}

template <class T>
T* AVLTree<T>::rightmost(T* n){
    if (n == nullptr)
        n = root;
    
    if (n == nullptr)
        return nullptr;
    
    else{
        if(n->right == nullptr)
            return n;
        else
            return rightmost(n->right);
        
    }
}

template <class T>
T* AVLTree<T>::next(T* n){
    if( n == nullptr)
        return nullptr;
    else{
        if(n->right != nullptr)
            return rightmost(n->right);
        if(n->parent != nullptr)
            return n->parent;
    }
    
    return nullptr;

}

template <class T>
T* AVLTree<T>::search(int key, T *start_node){
    
    if (root == nullptr)
        return nullptr;
    else{
        if (start_node == nullptr)
            start_node = root;
        
        if (start_node->key == key)
            return start_node;
        
        else{
            T *n = start_node;
            
            bool goLeft = n->key > key;
            n = goLeft ? (T*)n->left : (T*)n->right;
            
            if (n == nullptr)
                return nullptr;
            
            else
                return search(key, n);
        }
    }
}

template <class T>
T* AVLTree<T>::search_less(int key, T *start_node){
    
    if (root == nullptr)
        return nullptr;
    else{
        if (start_node == nullptr)
            start_node = root;
        
        if (start_node->key < key){
            //return right most node
            while (true) {
                if ((T*)start_node->right != nullptr)
                    start_node = (T*)start_node->right;
                
                else break;
            }
            return start_node;
        }
        
        else{
            T *n = start_node;
            
            bool goLeft = n->key > key;
            n = goLeft ? (T*)n->left : (T*)n->right;
            
            if (n == nullptr)
                return nullptr;
            
            else
                return search_less(key, n);
        }
    }
}

template <class T>
T* AVLTree<T>::search_more(int key, T *start_node){
    
    if (root == nullptr)
        return nullptr;
    else{
        if (start_node == nullptr)
            start_node = root;
        
        if (start_node->key > key){
            //return leftmost most node
            while (true) {
                if ((T*)start_node->left != nullptr)
                    start_node = (T*)start_node->left;
                
                else break;
            }
            return start_node;
        }
        
        else{
            T *n = start_node;
            
            bool goLeft = n->key > key;
            n = goLeft ? (T*)n->left : (T*)n->right;
            
            if (n == nullptr)
                return nullptr;
            
            else
                return search_more(key, n);
        }
    }
}

template <class T>
void AVLTree<T>::rebalance(T *n) {
    setBalance(n);
    
    if (n->balance == -2) {
        if (height((T*)(n->left->left)) >= height((T*)(n->left->right)))
            n = rotateRight(n);
        else
            n = rotateLeftThenRight(n);
    }
    else if (n->balance == 2) {
        if (height((T*)n->right->right) >= height((T*)n->right->left))
            n = rotateLeft(n);
        else
            n = rotateRightThenLeft(n);
    }
    
    if (n->parent != nullptr) {
        rebalance((T*)(n->parent));
    }
    else {
        root = n;
    }
}

template <class T>
T* AVLTree<T>::rotateLeft(T *a) {
    T *b = (T*)a->right;
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

template <class T>
T* AVLTree<T>::rotateRight(T *a) {
    T *b = (T*)a->left;
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

template <class T>
T* AVLTree<T>::rotateLeftThenRight(T *n) {
    n->left = rotateLeft((T*)n->left);
    return rotateRight(n);
}

template <class T>
T* AVLTree<T>::rotateRightThenLeft(T *n) {
    n->right = rotateRight((T*)n->right);
    return rotateLeft(n);
}

template <class T>
int AVLTree<T>::height(T *n) {
    if (n == nullptr)
        return -1;
    return 1 + std::max(height((T*)n->left), height((T*)n->right));
}

template <class T>
void AVLTree<T>::setBalance(T *n) {
    n->balance = height((T*)(n->right)) - height((T*)(n->left));
}

template <class T>
void AVLTree<T>::printBalance(T *n) {
    if (n != nullptr) {
        printBalance(n->left);
        std::cout << n->balance << " ";
        printBalance(n->right);
    }
}

template <class T>
AVLTree<T>::AVLTree() : root(nullptr) {}

template <class T>
AVLTree<T>::~AVLTree(void) {
    delete root;
}

template <class T>
T* AVLTree<T>::insert(int key, T* add_node) {
    
    if (root == nullptr) {
        root = add_node;
        return root;
    }
    else {
        T *n = root, *parent;
       
        while (true) {
            if (n->key == key)
                return n;
            
            parent = n;
            
            bool goLeft = n->key > key;
            n = goLeft ? (T*)n->left : (T*)n->right;
            
            
            if (n == nullptr) {
                if (goLeft)
                    parent->left = add_node;
                else
                    parent->right = add_node;
                
                //add parent to node
                add_node->parent = parent;
                
                rebalance(parent);
                break;
            }
        }
        return add_node;
    }
}

template <class T>
void AVLTree<T>::printBalance() {
    printBalance(root);
    std::cout << std::endl;
}

//template <class T>
//void AVLTree<T>::printPoints() {
//    printPoints(root);
//    std::cout << std::endl;
//}

//template <class T>
//void AVLTree<T>::printPoints(T *n) {
//    if (n != nullptr) {
//        printPoints(n->left);
//        std::cout << n->x << " " << n->y << "\n";
//        printPoints(n->right);
//    }
//}
