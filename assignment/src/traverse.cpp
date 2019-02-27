//
//  traverse.cpp
//  rapyuta
//
//  Created by tanyarags on 26/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#include "traverse.hpp"
///Path Functions//

Point LineSegment::check_intersect(LineSegment* seg){
    Point ip(-1,-1);
    
    if((point1->x == point2->x) && (seg->point1->x == seg->point2->x))
        return ip;
    
    if((point1->y == point2->y) && (seg->point1->y == seg->point2->y))
        return ip;
    
    if(point1->x == point2->x){ // vertical line segment
        if( seg->point1->x > point1->x && seg->point2->x < point1->x){
            ip.x = point1->x;
            ip.y = seg->point1->y;
        }
        if( seg->point1->x < point1->x && seg->point2->x > point1->x){
            ip.x = point1->x;
            ip.y = seg->point1->y;
        }
    }
    
    if(point1->y == point2->y){ // horizontal point
        if( seg->point1->y > point1->y && seg->point2->y < point1->y){
            ip.x = seg->point1->x;
            ip.y = point1->y;
        }
        if( seg->point1->y < point1->y && seg->point2->y > point1->y){
            ip.x = seg->point1->x;
            ip.y = point1->y;
        }
    }
    
    return ip;
}

bool Path::push(int x1, int y1, int x2, int y2){
    if (y1 == y2){ //horizonal
        int key = x1< x2 ? x1 : x2 ;//y1 //TODO: make it flexible
        PathNode* node = new PathNode(key, len_hor, x1, y1, x2, y2, nullptr);
        hor_root->insert(key, node);
        len_hor = len_hor + 1;
    }
    
    if (x1 == x2){ //vertical
        int key = x1< x2 ? x1 : x2 ;
        PathNode* node = new PathNode(key, len_ver, x1, y1, x2, y2, nullptr);
        ver_root->insert(key, node);
        len_ver = len_ver + 1;
    }
    return true;
}
void Path::print_tree(PathNode* node ){
    if (node != nullptr) {
        print_tree((PathNode*)node->left);
        node->print();
        print_tree((PathNode*)node->right);
    }
}

void Path::print_path(){
    print_tree(hor_root->root);
    print_tree(ver_root->root);
}

bool Path::check_end_point(int x, int y){
//checking only in horizontal tree
    PathNode* node = hor_root->search(x, nullptr);
    while(node!= nullptr){
        if (node->point1->y == y || node->point2->y == y)
            return true;
        
        node = (PathNode*)(hor_root->next(node));
    }
    return false;
}

std::list<Point> Traversal::find_intersection(AVLTree<PathNode> *hor_tree, AVLTree<PathNode> *ver_tree){
    std::list<Point> intersection_points;
    //important that horizontal tree is different from vertical tree

    int index_x = 1;
    PathNodeList* touched_nodes = nullptr;
    PathNodeList* end_ptr = nullptr ;


    PathNode* node =  (PathNode*)(hor_tree->leftmost(hor_tree->root)); //startnode

    //init index_x: to avoid starting from 1
    index_x = node->key;

    while(index_x <= maze->cols){
        //add nodes with index_x to list
        while( node!= nullptr && node->key == index_x){

            PathNodeList* new_node = new PathNodeList(node);
            if (touched_nodes == nullptr){
                touched_nodes = new_node;
                end_ptr = touched_nodes;
            }

            else{
                end_ptr->next = new_node;
                end_ptr = new_node;
            }
            node = (PathNode*)(hor_tree->next(node));
        }

        //vertical node where index_x found
        PathNode* ver_node = ver_tree->search(index_x, nullptr);
        
        if ( ver_node != nullptr)
        {
            end_ptr = touched_nodes;
            
            while(ver_node != nullptr && ver_node->key == index_x){
                end_ptr = touched_nodes;
                while(end_ptr != nullptr){
                    
                    Point intersection_p = check_intersect(end_ptr->pathnode, ver_node);
                    if (intersection_p.x != -1){
                        //cout<<"\nPoint found " << intersection_p.x << " " << intersection_p.y;
                        intersection_points.push_back(intersection_p);
                    }
                    
                    //perform seiving
                    if( !valid_segment((LineSegment*)end_ptr->pathnode, index_x)){
                        
                        if(end_ptr == touched_nodes) //start pointer
                            touched_nodes = end_ptr->next;
                        
                        PathNodeList* tmp_ptr = end_ptr;
                        end_ptr = end_ptr->next;
                        delete tmp_ptr;
                    }
                    else
                        end_ptr = end_ptr->next;
                }
                ver_node = (PathNode*)(ver_tree->next(ver_node)); //iter++
            }
        }
        
        index_x = index_x +1;
    }
    return intersection_points;
}

bool Traversal::valid_segment(LineSegment* seg, int indx){
    if(seg->point1->x < indx && seg->point2->x < indx)
        return false;
    return true;
}

Point Traversal::check_intersect(PathNode* node1, PathNode* node2){
    Point ip(-1,-1);
    
    ip = ((LineSegment*)node1)->check_intersect(((LineSegment*)node2));
    
    return ip;
}

bool mycomparison (Point first, Point second)
{ return ( int(first.x + first.y) <int(second.x + second.y) ); }


std::list<Point> Traversal::find_intersection(Path* path1, Path * path2){
    std::list<Point> l1 =  find_intersection(path1->hor_root, path2->ver_root);
    std::list<Point> l2 = find_intersection(path2->hor_root, path1->ver_root);

    l1.sort(mycomparison);
    l2.sort(mycomparison);
    l1.merge(l2, mycomparison);
    return l1;

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
        //cout<<"\nLine Segment from (" <<x1 << "," << y1<< ") ("<<x2 << "," << y2 << ")";
        
        path->push(x1, y1, x2, y2);
    }
    
    return path;
}
