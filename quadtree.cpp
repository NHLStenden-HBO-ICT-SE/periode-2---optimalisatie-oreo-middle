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
    
    if (node == root.get()) return node; // Return nothing if node is root

    //The Directions and target nodeIndex --which is used to check if its coming from the right node and going to the right node (based on neighbour direction)
    int hDir = 0, vDir = 0, tNode1 = 0, tNode2 = 0; 
    switch (dir) {
        //Horizontal and vertical
    case 1:
        hDir = 2, vDir = 3, tNode1 = 0, tNode2 = 1;
        break;
    case 3:
        hDir = 1, vDir = 3, tNode1 = 0, tNode2 = 2;
        break;
    case 4:
        hDir = 0, vDir = 2, tNode1 = 1, tNode2 = 3;
        break;
    case 6:
        hDir = 0, vDir = 1, tNode1 = 2, tNode2 = 3;
        break;

    //Diagonal --There is only 1 possible node (dir- and target node) when checking diagonaly
    vDir = -1, tNode2 = -1;
    case 0:
        hDir = 3, tNode1 = 0;
        break;
    case 2:
        hDir = 2, tNode1 = 1;
        break;
    case 5:
        hDir = 1, tNode1 = 2;
        break;
    case 7:
        hDir = 0, tNode1 = 3;
        break;
    }

    // No need to find parent when neigbor is in same quadrant
    if (node->parent->children[hDir].get() == node) {
        return node->parent->children[tNode1].get();
    }
    else if (vDir != -1 && node->parent->children[vDir].get() == node) {
        return node->parent->children[tNode2].get();
    }

    // If neighbor is not in same quadrant search in parent quadrant
    Node* neighborCandidate = findNeighbourNodes(node->parent, dir);
    if (neighborCandidate == nullptr || neighborCandidate->leafnode)
        return neighborCandidate;

    if (neighborCandidate->children[tNode1].get() == node) {
        return neighborCandidate->children[hDir].get();
    }
    else if (vDir != -1)
        return neighborCandidate->children[vDir].get();
    
}

vector<vec2> Quadtree::findNeighbourPoints(Node* node, int dir) {
    if (node == nullptr) return {-1};

    vector<Node*> smallNeighbours = {node};
    vector<Node*> candidateNeighbours;
    vector<vec2> candidatePoints;

    int hDir = 0, vDir = 0;
    switch (dir) {
        //Horizontal and vertical
        case 1:
            hDir = 2, vDir = 3;
            break;
        case 3:
            hDir = 1, vDir = 3;
            break;
        case 4:
            hDir = 0, vDir = 2;
            break;
        case 6:
            hDir = 0, vDir = 1;
            break;

        //Diagonal
        case 0:
            hDir = 3, vDir = -1;
            break;
        case 2:
            hDir = 2, vDir = -1;
            break;
        case 5:
            hDir = 1, vDir = -1;
            break;
        case 7:
            hDir = 0, vDir = -1;
            break;
    }

    while (smallNeighbours.size() > 0) {
        if (smallNeighbours[0]->leafnode && !smallNeighbours[0]->empty) {
            candidateNeighbours.push_back(smallNeighbours[0]);
        }
        else if (smallNeighbours[0]->leafnode && smallNeighbours[0]->empty) { // Search smallNeighbour in parent node because current leafnode is empty
            for (int i = 0; i < smallNeighbours[0]->parent->children.size(); i++) {
                if (smallNeighbours[0]->parent->children[i].get()->empty != true) {
                    smallNeighbours.push_back(smallNeighbours[0]->parent->children[i].get());
                }
            }
        }
        else {
            smallNeighbours.push_back(smallNeighbours[0]->children[hDir].get());
            if (vDir != -1)
                smallNeighbours.push_back(smallNeighbours[0]->children[vDir].get());
        }
        smallNeighbours.erase(smallNeighbours.begin());
    }
    for (Node* node : candidateNeighbours) {
        for (vec2& point : node->points) {
            candidatePoints.push_back(point);
        }
    }

    return candidatePoints;
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
