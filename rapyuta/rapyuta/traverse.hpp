//
//  traverse.hpp
//  rapyuta
//
//  Created by tanyarags on 26/02/19.
//  Copyright © 2019 tanyarags. All rights reserved.
//

#ifndef traverse_hpp
#define traverse_hpp

#include <stdio.h>
#include <list>
#include "maze.hpp"

struct Point {
    int x, y;
    Point(int x,int y): x(x), y(y){}
};

struct LineSegment{
    int seg_id;
    Point *point1, *point2;
    
    LineSegment(int seg_id, int x1, int y1, int x2, int y2): seg_id(seg_id){
        point1 = new Point(x1, y1);
        point2 = new Point(x2, y2);
    };
    
    ~LineSegment(){
        delete point1;
        delete point2;
    }
    
    Point check_intersect(LineSegment* seg);
    
    void print(){
        //cout<< "\nPoint1 (" << point1->x << "," << point1->y << ") Point2 (" << point2->x << ","<<point1->y << ")";
    }
};

struct PathNode: public LineSegment, public AVLNode{
    PathNode(int k, int node_id, int x1, int y1, int x2, int y2, PathNode *p): LineSegment(node_id, x1, y1, x2, y2), AVLNode(k, (AVLNode*)p){};
    
    ~PathNode() {}
};

struct PathNodeList{
    PathNode* pathnode;
    PathNodeList* next;
    PathNodeList(PathNode* pathnode): pathnode(pathnode), next(nullptr){};
    ~PathNodeList(){
        delete pathnode;
        delete next;
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
    
    ~Path(){
        delete hor_root;
        delete ver_root;
    }
    
    bool push(int x1, int y1, int x2, int y2);
    void print_path();
    void print_tree(PathNode* node);
    bool check_end_point(int x, int y);
};

struct  Traversal {
    Maze* maze;
    
    Traversal(Maze* maze): maze(maze){}
    ~Traversal(){}
    
    Base* next_node(Base* node, int direction);
    int emerging_direction(Base* node, int in_direction);
    
    std::list<Point> find_intersection(Path* path1, Path * path2);
    std::list<Point> find_intersection(AVLTree<PathNode> *hor_tree, AVLTree<PathNode> *ver_tree);
    Point check_intersect(PathNode* node1, PathNode* node2);
    bool valid_segment(LineSegment* seg, int indx);
    
    Path* traverse(Base* node, int direction);
    Path* traverse_start(){
        return traverse((Base*)(maze->start_node->wallpoint), 1);
    }
    
    Path* traverse_end(){
        return traverse((Base*)(maze->end_node->wallpoint), 3);
    }
    
};

#endif /* traverse_hpp */
