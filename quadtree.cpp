#include "precomp.h"
#include "quadtree.h"

namespace Tmpl8 {

int Quadtree::getQuadrant(vec2& p, vec2 min, vec2 max) {
    //Middle position of quadtree
    int midx = (min.x - max.x) / 2;
    int midy = (min.y - max.y) / 2;

    if (p.x <= midx && p.y <= midy) return 0; //Upper left
    if (p.x >= midx && p.y <= midy) return 1; //Upper right
    if (p.x <= midx && p.y >= midy) return 2; //Lower left
    if (p.x >= midx && p.y >= midy) return 3; //Lower right

    //Returns none if quadrant not found
    return -1;
}

void Quadtree::add(Node* node, vec2& p, vec2 min, vec2 max, int depth) {
    
    if (node->leafnode && node->empty) {
        node->empty = false;
        node->points.push_back(p);
        return;
    }

    //Add node_point to other node
    if (!node->leafnode) {
        int i = getQuadrant(p, min, max);
        if(i != -1)
            add(node->children[i].get(), p, min, max, depth + 1);
    }

    //If node_point already exists, return
    //if (!node->empty && p == node->points) return;

    //If leafnode and not empty
    if (node->leafnode && !node->empty) {
        //Check if quadtree is not at its max threshold or depth
        if (depth >= MaxDepth || node->points.size() < Threshold) {
            node->points.push_back(p);
        }
        else { //split current quadtree in 4
            split(node, min, max); 
            add(node, p, min, max, depth);
        }
    }
}

void Quadtree::split(Node* node, vec2 min, vec2 max) {
    node->leafnode = false;
    for (auto& child : node->children) { //Initalize 4 children
        child = std::make_unique<Node>();
    }

    for(vec2 point : node->points) {
        int i = getQuadrant(point, min, max);
        if (i != -1)
            node->children[i]->points.push_back(point);
    }
}

void Quadtree::search(Node* node, vec2& p, vec2 min, vec2 max) {
    //Search for point in quadtree
}

//TODO: add remove function

};
