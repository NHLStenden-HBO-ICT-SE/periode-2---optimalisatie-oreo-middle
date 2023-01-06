#include "precomp.h"
#include "quadtree.h"

namespace Tmpl8 {

int Quadtree::getQuadrant(vec2& p, Boundries* boundries) {
    //Middle position of quadtree
    int midx = boundries->minx + ((boundries->maxx - boundries->minx) / 2);
    int midy = boundries->miny + ((boundries->maxy - boundries->miny) / 2);

    if (p.x <= midx && p.y <= midy) return 0; //Upper left
    if (p.x >= midx && p.y <= midy) return 1; //Upper right
    if (p.x <= midx && p.y >= midy) return 2; //Lower left
    if (p.x >= midx && p.y >= midy) return 3; //Lower right

    //Returns none if quadrant not found
    return -1;
}

void Quadtree::newBoundries(Boundries* boundries, int q) {
    vec2 min = (boundries->minx, boundries->miny);

    int midx = boundries->minx + ((boundries->maxx - boundries->minx) / 2);
    int midy = boundries->miny + ((boundries->maxy - boundries->miny) / 2);

    switch (q) {
        case 0: //Upperleft
            boundries->maxx = midx;
            boundries->maxy = midy;
            break;

        case 1: //Upper right
            boundries->minx = midx;
            boundries->maxy = midy;
            break;

        case 2: //Lower left
            boundries->maxx = midx;
            boundries->miny = midy;
            break;

        case 3: //Lower right
            boundries->minx = midx;
            boundries->miny = midy;
            break;
    }
}

void Quadtree::add(Node* node, vec2& p, Boundries* boundries, int depth) { 
    //For initializing first node
    if (node->boundries == nullptr) { 
        node->boundries = std::make_unique<Boundries>();
    }

    if (node->leafnode && node->empty) {
        node->empty = false;
        node->points.push_back(p);
        return;
    }

    //Add node_point to other node
    if (!node->leafnode) {
        int i = getQuadrant(p, boundries);
        if (i != -1) {
            add(node->children[i].get(), p, node->children[i]->boundries.get(), depth + 1);
        }
            
    }

    //If leafnode and not empty
    if (node->leafnode && !node->empty) {
        //Check if quadtree is not at its max threshold or depth
        if (depth >= MaxDepth || node->points.size() < Threshold) {
            node->points.push_back(p);
        }
        else { //split current quadtree in 4
            split(node);
            add(node, p, boundries, depth);
        }
    }
}

void Quadtree::split(Node* node) {
    node->leafnode = false;
    int i = 0;
    for (auto& child : node->children) { //Initalize 4 children
        child = std::make_unique<Node>();
        child->boundries = std::make_unique<Boundries>();
        child->boundries.get()->minx = node->boundries.get()->minx;
        child->boundries.get()->miny = node->boundries.get()->miny;
        child->boundries.get()->maxx = node->boundries.get()->maxx;
        child->boundries.get()->maxy = node->boundries.get()->maxy;
        newBoundries(child->boundries.get(), i);
        child->parent = node;
        i++;
    }

    vector<vec2> newPoints;

    for(vec2& point : node->points) {
        int i = getQuadrant(point, node->boundries.get());
        if (i != -1) {
            node->children[i]->points.push_back(point);
        }
    }
    node->points = std::move(newPoints);
}

vector<vec2> Quadtree::search(Node* node, vec2& p) {

    if (!node->leafnode) {
        int i = getQuadrant(p, node->boundries.get());
        if (i != -1 && node->children[i].get() != nullptr) {
            return search(node->children[i].get(), p);
        }
    }

    vector<Node*> neighbournodes;
    vector<vec2> neighbourpoints;
    for (int dir = 0; dir < 8; dir++) {
        neighbournodes.push_back(findNeighbourNodes(node, dir));

        vector<vec2> tempNeighbourpoints = findNeighbourPoints(neighbournodes[dir], dir);
        for (vec2& point : tempNeighbourpoints) {
            neighbourpoints.push_back(point);
        }
    }

    return neighbourpoints;
}

Quadtree::Node* Quadtree::findNeighbourNodes(Node* node, int dir) {
    
    if (dir == 1) { // 1 is up
        if (node == root.get()) return; // Return nothing if node is root

        // No need to find parent when neigbor is in same quadrant
        if (node->parent->children[2].get() == node) {
            return node->parent->children[0].get();
        }
        else if (node->parent->children[3].get() == node) {
            return node->parent->children[1].get();
        }

        // If neighbor is not in same quadrant search in parent quadrant
        Node* neighborCandidate = findNeighbourNodes(node->parent, dir);
        if (neighborCandidate == nullptr || neighborCandidate->leafnode)
            return neighborCandidate;

        if (neighborCandidate->children[0].get() == node) {
            return neighborCandidate->children[2].get();
        }
        else
            return neighborCandidate->children[3].get();
    }
    // --TODO: loop through all directions instead of only '1'
    
}

vector<vec2> Quadtree::findNeighbourPoints(Node* node, int dir) {

}

void Quadtree::clear(Node* node) {
    node->points.clear();
    node->leafnode = true;
    node->empty = true;
    for (int i = 0; i < node->children.size() -1; i++) {
        if (node->children[i] != nullptr) {
            clear(node->children[i].get());
            node->children[i].reset();
        }
    }
}

//TODO: remove function

};
