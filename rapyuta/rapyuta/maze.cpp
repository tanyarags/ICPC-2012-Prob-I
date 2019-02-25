//
//  maze.cpp
//  rapyuta
//
//  Created by tanyarags on 25/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#include "maze.hpp"

bool Path::push(int x1, int y1, int x2, int y2){
    PathNode* node = new PathNode(x1,y1,x2,y2);
    if (x1 == x2){ //horizonal
        _push(node, hor_root);
        len_hor = len_hor + 1;
    }
    
    if (y1 == y2){ //vertical
        _push(node, ver_root);
        len_ver = len_ver + 1;
    }
    return true;
}

bool Path::_push(PathNode* node, PathNode* root){
    
    if (root == nullptr)
        root = node;
    
    int weight = root->x1 + root->y1;
    int node_weight = node->x1 + node->y1;
    
    if(weight > node_weight){
        PathNode *ptr = root;
        root = node;
        root->next = ptr;
    }
    else
        _push(node, root->next);
    
    return true;
}

void Path::print_path(){
    PathNode *node = hor_root;
    while(node != nullptr){
        cout<<node->x1<<" "<<node->y1<<" "<<node->x2<<" "<<node->y2<<" ";
        node = node->next;
    }
    node = ver_root;
    while(node != nullptr){
        cout<<node->x1<<" "<<node->y1<<" "<<node->x2<<" "<<node->y2<<" ";
        node = node->next;
    }
    
}


bool Maze::add_mirror(int type, int x, int y){
    int key_x = x;
    int key_y = y;
    
    //create a mirror
    Mirror* m = new Mirror(type,x,y);//new mirror
    
    WallNode *w_node_x = get_wall_node(row_tree, x, 0);
    WallNode *w_node_y = get_wall_node(col_tree, 0, y);
    
    AVLTree<MirrorNode> *m_tree_x = get_mirror_tree(w_node_x);
    AVLTree<MirrorNode> *m_tree_y = get_mirror_tree(w_node_y);
    
    make_connections(m, key_x, key_y, w_node_x, m_tree_x);
    make_connections(m, key_y, key_x, w_node_y, m_tree_y);
    
    return true;
}

bool Maze::make_connections(Mirror* m, int key, int key_c, WallNode* w_node, AVLTree<MirrorNode>* m_tree){
    
    int type = m->type, x = m->x, y = m->y;
    
    MirrorNode *m_node = new MirrorNode(key_c, type, x, y, m );
    if (m_tree->root == nullptr)//no mirror exist along this
        m_node = m_tree->insert(key_c, m_node);

    MirrorNode *less_node = m_tree->search_less(key_c);
    MirrorNode *more_node = m_tree->search_more(key_c);
    
    //connect the node just less than curr node//
    if (less_node != nullptr)
        m_node->mirror->add_neighbor((Base*)(less_node->mirror));
    
    else
        m_node->mirror->add_neighbor((Base*)(w_node->wallpoint));
    
    //connect the node just more than curr node//
    if (more_node != nullptr)
        m_node->mirror->add_neighbor((Base*)(more_node->mirror));
    
    return true;
}
WallNode* Maze::get_wall_node(AVLTree<WallNode>* wall_tree, int x, int y){
    int key = x + y;

    //get wall node pointer for this mirror (from Wall Tree)
    WallNode* w_node = wall_tree->search(key);
    if (w_node == nullptr){
        //wall node doesnt exist, add new
        w_node = new WallNode(key, 0, x, y);
        w_node = wall_tree->insert(key, w_node);
    }

    return w_node;
}

AVLTree<MirrorNode>* Maze::get_mirror_tree(WallNode* w_node){
    //int x= m->x, y = m->y, type = m->type;
    //get mirror tree pointer for this mirror (from wallNode)
    AVLTree<MirrorNode>* mirror_tree = w_node->mirror_tree;
    if (mirror_tree == nullptr) {//no mirrors exist, add current
        mirror_tree = new AVLTree<MirrorNode>();
        w_node->mirror_tree = mirror_tree;
    }

    return mirror_tree;
}

int Maze::emerging_direction(Base* node, int in_direction){
    int type = node->type;
    int return_dir = -1;
    
    if (type ==0){
        int x = node->x, pos_direction;
        int y = node->y;
        
        if (x == 0) pos_direction = 1; //vertical wall node
        else if (y == 0) pos_direction = 2; //horizontal wall node
        else if (x == (cols + 1)) pos_direction = 3;
        else if (y == (rows + 1)) pos_direction = 0;
        else pos_direction = -1;
        
        if (in_direction == pos_direction) return_dir = in_direction;
    }
    
    else if(type == -1){//case '/'
        int map[4] = {3, 2, 1, 0};
        return_dir = map[in_direction];
    }
    
    else if (type == 1){//case '\'
        int map[4] = {1, 0, 3, 2};
        return_dir = map[in_direction];
    }
    
    return return_dir;
}

Base* Maze::next_node(Base* node, int emerge_direction){
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

Path* Maze::traverse(Base* node, int direction){
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
                case 1: x2 = cols;
                    break;
                case 2: y2 = rows;
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
        path->push(x1, y1, x1, y2);
    }
    
    return path;
}

