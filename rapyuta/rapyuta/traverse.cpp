//
//  traverse.cpp
//  rapyuta
//
//  Created by tanyarags on 26/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#include "traverse.hpp"
///Path Functions//

bool Path::push(int x1, int y1, int x2, int y2){
    if (y1 == y2){ //horizonal
        int key = x1;//y1 //TODO: make it flexible
        PathNode* node = new PathNode(key, len_hor, x1, y1, x2, y2, nullptr);
        hor_root->insert(key, node);
        len_hor = len_hor + 1;
    }
    
    if (x1 == x2){ //vertical
        int key = x1;
        PathNode* node = new PathNode(key, len_ver, x1, y1, x2, y2, nullptr);
        ver_root->insert(key, node);
        len_ver = len_ver + 1;
    }
    return true;
}
void Path::print_tree(PathNode* node ){
    if (node != nullptr) {
        print_tree(node->left);
        node->print();
        print_tree(node->right);
    }
}

void Path::print_path(){
    print_tree(hor_root->root);
    print_tree(ver_root->root);
}

//void Path::find_intersection(AVLTree<PathNode> *hor_tree, AVLTree<PathNode> *ver_tree){
//    std::list<Point> intersection_points;
//    //important that horizontal tree is different from vertical tree
//
//    int index_x = 1;
//    PathNodeList* touched_nodes = nullptr;
//    PathNodeList* adding_ptr = nullptr ;
//
//
//    PathNode* node =  (PathNode*)(hor_tree->root->leftmost()); //startnode
//
//    //init index_x: to avoid starting from 1
//    index_x = node->key;
//
//    while(node != nullptr){
//        //add nodes with index_x to list
//        while( node->key == index_x){
//
//            PathNodeList* new_node = new PathNodeList(node);
//            if (touched_nodes == nullptr){
//                touched_nodes = new_node;
//                adding_ptr = touched_nodes;
//            }
//
//            else{
//                adding_ptr->next = new_node;
//                adding_ptr = new_node;
//            }
//            node = (PathNode*)(node->next());
//        }
//
//
//        //ver_tree->search(index_x);
//        //remove if line ended
//
//    }
//    
//    //return intersection_points;
//}

void Traversal::find_intersection(Path* path1, Path * path2){
        //to be written
}


int Traversal::emerging_direction(Base* node, int in_direction){
    int type = node->type;
    int return_dir = -1;
    
    if (type ==0){
        int x = node->x, pos_direction;
        int y = node->y;
        
        if (x == 0) pos_direction = 1; //vertical wall node
        else if (y == 0) pos_direction = 2; //horizontal wall node
        else if (x == (maze->cols + 1)) pos_direction = 3;
        else if (y == (maze->rows + 1)) pos_direction = 0;
        else pos_direction = -1;
        
        if (in_direction == pos_direction) return_dir = in_direction;
    }
    
    else if(type == -1){//case '/'
        int map[4] = {1,0, 3, 2};
        return_dir = map[in_direction];
    }
    
    else if (type == 1){//case '\'
        int map[4] = {3, 2, 1, 0};
        return_dir = map[in_direction];
    }
    
    return return_dir;
}

Base* Traversal::next_node(Base* node, int emerge_direction){
    if (node != nullptr){
        int type = node->type;
        
        if (emerge_direction != -1){
            if (type == 0)
                return ((WallPoint*)node)->closest_mirror;
            else
                return ((Mirror*)node)->directions[emerge_direction];
        }
    }
    return nullptr;
}

Path* Traversal::traverse(Base* node, int direction){
    Path* path = new Path();
    
    while(node != nullptr){
        int x1=node->x, y1 = node->y;
        int x2 = x1, y2 = y1;
        direction = emerging_direction(node, direction);
        
        node = next_node(node, direction);
        
        if (node == nullptr){
            switch (direction) {
                case 0: y2 = 0;
                    break;
                case 1: x2 = maze->cols + 1;
                    break;
                case 2: y2 = maze->rows + 1;
                    break;
                case 3: x2 = 0;
                    break;
                default:
                    break;
            }
        }
        
        else{
            x2 = node->x;
            y2 = node->y;
        }
        cout<< "Line Segment from (" <<x1 << "," << y1<< ") ("<<x2 << "," << y2 << ")";
        
        path->push(x1, y1, x2, y2);
    }
    
    return path;
}