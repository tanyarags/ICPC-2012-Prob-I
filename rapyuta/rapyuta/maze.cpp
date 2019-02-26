//
//  maze.cpp
//  rapyuta
//
//  Created by tanyarags on 25/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#include "maze.hpp"

///Mirror functions////
bool Mirror::add_neighbor(Base* neighbor){
    int direction = get_direction(neighbor);
    
    if (direction != -1){
        // changed to curr new node
        if( neighbor->type ==0) //Wall point
            directions[direction] = ((WallPoint*)neighbor)->closest_mirror;
        
        else
            directions[direction] = ((Mirror*)neighbor)->directions[direction];
        
        directions[(direction+2)%4] = neighbor; //curr node points to neighbor.
        
        //changed for neighbor
        if( neighbor->type ==0){//Wall point
            ((WallPoint*)neighbor)->closest_mirror = (Base*)this;
            cout << "\nClosest Mirror updated " << ((WallPoint*)neighbor)->x << " " << ((WallPoint*)neighbor)->y;
        }
        else{
            ((Mirror*)neighbor)->directions[direction] = (Base*)this;
            cout << "\nMirror neighbor updated " << ((Mirror*)neighbor)->x << " " << ((Mirror*)neighbor)->y;
        }
    }
    return true;
}

int Mirror::get_direction(Base* node){
    int ref_x = node->x, ref_y = node->y;
    
    // 0, 1, 2, 3: north, east south, west
    int direction = -1;
    
    if (ref_x == x)
        direction = y < ref_y? 0: 2 ;//north, south
    
    if(ref_y == y)
        direction = x < ref_x? 3: 1 ;//west, east
    
    return direction;
}


///Maze functions///
bool Maze::add_mirror(int type, int x, int y){
    int key_x = x;
    int key_y = y;
    
    cout << "\nMaking connections for " << x << " " <<y;
    //create a mirror
    Mirror* m = new Mirror(type,x,y);//new mirror
    
    WallNode *w_node_x = get_wall_node(row_tree, x, 0);
    WallNode *w_node_y = get_wall_node(col_tree, 0, y);
    
    AVLTree<MirrorNode> *m_tree_x = get_mirror_tree(w_node_x);
    AVLTree<MirrorNode> *m_tree_y = get_mirror_tree(w_node_y);
    
    make_connections(m, key_x, key_y, w_node_x, m_tree_x);
    make_connections(m, key_y, key_x, w_node_y, m_tree_y);
    
    //special case. when mirror is in last row.
    if (m->y == rows and added_last_row == false){
        m->add_neighbor((Base*)(end_node->wallpoint));
        added_last_row = true;
    }
    
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
    
    cout << "\nFetched Wall " << ((Base*)w_node)->x << " " << ((Base*)w_node)->y;
    return w_node;
}

AVLTree<MirrorNode>* Maze::get_mirror_tree(WallNode* w_node){
    AVLTree<MirrorNode>* mirror_tree = w_node->mirror_tree;
    if (mirror_tree == nullptr) {//no mirrors exist, add current
        mirror_tree = new AVLTree<MirrorNode>();
        w_node->mirror_tree = mirror_tree;
    }

    return mirror_tree;
}


