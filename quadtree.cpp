#include "precomp.h"
#include "quadtree.h"

namespace Tmpl8 {

int Quadtree::getQuadrant(vec2& point, Boundries* boundries) {
    //Middle position of quadtree
    int midx = boundries->minx + ((boundries->maxx - boundries->minx) / 2);
    int midy = boundries->miny + ((boundries->maxy - boundries->miny) / 2);

    if (point.x <= midx && point.y <= midy) return 0; 
    if (point.x >= midx && point.y <= midy) return 1; 
    if (point.x <= midx && point.y >= midy) return 2; 
    if (point.x >= midx && point.y >= midy) return 3;
    //  --- ---
    // | 0 | 1 |
    //  --- --- 
    // | 2 | 3 |
    //  --- ---

    //Returns none if quadrant not found
    return -1;
}

void Quadtree::newBoundries(Boundries* boundries, int quadrant) {
    vec2 min = (boundries->minx, boundries->miny);

    int midx = boundries->minx + ((boundries->maxx - boundries->minx) / 2);
    int midy = boundries->miny + ((boundries->maxy - boundries->miny) / 2);

    switch (quadrant) {
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

void Quadtree::add(Node* node, vec2& point, Boundries* boundries, int depth) { 
    //For initializing first node boundries
    if (node->boundries == nullptr) { 
        node->boundries = std::make_unique<Boundries>();
    }

    //No need to search for another node if it can be put into this one
    if (node->leafnode && node->empty) {
        node->empty = false;
        node->points.push_back(point);
        return;
    }

    //Add point to corresponding child node
    if (!node->leafnode) {
        int i = getQuadrant(point, boundries);
        if (i != -1) {
            add(node->children[i].get(), point, node->children[i]->boundries.get(), depth + 1);
        }
            
    }

    //If leafnode and not empty
    if (node->leafnode && !node->empty) {
        //Check if quadtree is not at its max threshold or depth
        if (depth >= MaxDepth || node->points.size() < Threshold) {
            node->points.push_back(point);
        }
        else { //split current node in 4 other nodes and divide points into those nodes
            split(node);
            add(node, point, boundries, depth);
        }
    }
}

void Quadtree::split(Node* node) {
    node->leafnode = false;

    int i = 0;
    for (auto& child : node->children) { //Initalize 4 children with default values
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

    //Move points of current node into corresponding child node
    for(vec2& point : node->points) {
        int i = getQuadrant(point, node->boundries.get());
        if (i != -1) {
            node->children[i]->points.push_back(point);
        }
    }
    node->points = std::move(newPoints);
}

vector<vec2> Quadtree::search(Node* node, vec2& point) {

    //recursive search for the smallest node that should contains position of point
    if (!node->leafnode) {
        char i = getQuadrant(point, node->boundries.get());
        if (i != -1 && node->children[i].get() != nullptr) {
            return search(node->children[i].get(), point);
        }
    }

    vector<Node*> neighbournodes;
    vector<vec2> neighbourpoints;

    // Check neighbour nodes for points and add them to a list of points. Directions are horizontal and diagonal
    // $ is the node in question 
    //  --- --- ---
    // | 0 | 1 | 2 |
    //  --- --- --- 
    // | 3 | $ | 4 |    
    //  --- --- --- 
    // | 5 | 6 | 7 |
    //  --- --- ---
    for (int dir = 0; dir < 8; dir++) {

        //Find neighbouring nodes of same size of greater size first
        neighbournodes.push_back(findNeighbourNodes(node, dir));

        //Find smaller sized neighbours that are not empty
        vector<vec2> tempNeighbourpoints = findNeighbourPoints(neighbournodes[dir], dir);

        for (vec2& point : tempNeighbourpoints) {
            //Add points of non-empty neighbour node to list
            neighbourpoints.push_back(point);
        }
    }

    return neighbourpoints;
}

Quadtree::Node* Quadtree::findNeighbourNodes(Node* node, int dir) {
    
    if (node == root.get()) return node; // Return node, root has no neighbours

    //The origin quadrant numbers and target quadrant numbers, corresponding to its direction position
    char origin_quadrant1 = 0, origin_quadrant2 = 0, target_quadrant1 = 0, target_quadrant2 = 0;
    switch (dir) {
        //Horizontal and vertical
    case 1:
        origin_quadrant1 = 2, origin_quadrant2 = 3, target_quadrant1 = 0, target_quadrant2 = 1;
        break;
    case 3:
        origin_quadrant1 = 1, origin_quadrant2 = 3, target_quadrant1 = 0, target_quadrant2 = 2;
        break;
    case 4:
        origin_quadrant1 = 0, origin_quadrant2 = 2, target_quadrant1 = 1, target_quadrant2 = 3;
        break;
    case 6:
        origin_quadrant1 = 0, origin_quadrant2 = 1, target_quadrant1 = 2, target_quadrant2 = 3;
        break;

    //Diagonal --There is only 1 possible node when checking diagonaly for neighbours
    origin_quadrant2 = -1, target_quadrant2 = -1;
    case 0:
        origin_quadrant1 = 3, target_quadrant1 = 0;
        break;
    case 2:
        origin_quadrant1 = 2, target_quadrant1 = 1;
        break;
    case 5:
        origin_quadrant1 = 1, target_quadrant1 = 2;
        break;
    case 7:
        origin_quadrant1 = 0, target_quadrant1 = 3;
        break;
    }

    // No need to find parent when neigbor is in same quadrant
    if (node->parent->children[origin_quadrant1].get() == node) {
        return node->parent->children[target_quadrant1].get();
    }
    else if (origin_quadrant2 != -1 && node->parent->children[origin_quadrant2].get() == node) {
        return node->parent->children[target_quadrant2].get();
    }

    // If neighbor is not in same quadrant, search further in parent quadrant for neighbournodes
    Node* neighborCandidate = findNeighbourNodes(node->parent, dir);
    if (neighborCandidate == nullptr || neighborCandidate->leafnode)
        return neighborCandidate;

    // Return self if there is no possible neighbour
    if (neighborCandidate == root.get())
        return node;

    if (neighborCandidate->children[target_quadrant1].get() == node) {
        return neighborCandidate->children[origin_quadrant1].get();
    }
    else if (origin_quadrant2 != -1)
        return neighborCandidate->children[origin_quadrant2].get();
    
}

vector<vec2> Quadtree::findNeighbourPoints(Node* node, int dir) {
    if (node == nullptr) return {-1};

    vector<Node*> smallNeighbours = {node};
    vector<Node*> candidateNeighbours;
    vector<vec2> candidatePoints;

    char origin_quadrant1 = 0, origin_quadrant2 = 0;
    switch (dir) {
        //Horizontal and vertical
        case 1:
            origin_quadrant1 = 2, origin_quadrant2 = 3;
            break;
        case 3:
            origin_quadrant1 = 1, origin_quadrant2 = 3;
            break;
        case 4:
            origin_quadrant1 = 0, origin_quadrant2 = 2;
            break;
        case 6:
            origin_quadrant1 = 0, origin_quadrant2 = 1;
            break;

        //Diagonal --origin_quadrant2 is always -1 because diagonal only has 1 node we need to check
        case 0:
            origin_quadrant1 = 3, origin_quadrant2 = -1;
            break;
        case 2:
            origin_quadrant1 = 2, origin_quadrant2 = -1;
            break;
        case 5:
            origin_quadrant1 = 1, origin_quadrant2 = -1;
            break;
        case 7:
            origin_quadrant1 = 0, origin_quadrant2 = -1;
            break;
    }

    // Continues until smallNeighbours is empty
    while (smallNeighbours.size() > 0) {

        // For each iteration check the first element of smallNeighbours for leafnode
        if (smallNeighbours[0]->leafnode) { 

            // When Node contains points it is added as a candidate neighbour node to a list
            if(!smallNeighbours[0]->empty) 
                candidateNeighbours.push_back(smallNeighbours[0]);
            // If not, see if the node is the same as one of the origin quadrants
            else if (smallNeighbours[0]->parent->children[origin_quadrant1].get() == smallNeighbours[0] || 
                    (origin_quadrant2 != -1 && smallNeighbours[0]->parent->children[origin_quadrant2].get() == smallNeighbours[0])){
                // Find a new neighbour of this node with its direction number and add that at the end of smallNeighbours list
                Node* neighbour = findNeighbourNodes(smallNeighbours[0], dir);
                smallNeighbours.push_back(neighbour);
            }
        }
        else { // Not a leafnode --add 2 children to be checked again in this while loop until a leafnode is found
            smallNeighbours.push_back(smallNeighbours[0]->children[origin_quadrant1].get());
            if (origin_quadrant2 != -1)
                smallNeighbours.push_back(smallNeighbours[0]->children[origin_quadrant2].get());
        }

        //Removes node of current iteration (which is always the first in list) no matter the outcome
        smallNeighbours.erase(smallNeighbours.begin());
    }

    // Extract points from all the candidate nodes
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
            nodePool.push_back(std::move(node->children[i]));
            node->children[i] = nullptr;
        }
    }
}

};
