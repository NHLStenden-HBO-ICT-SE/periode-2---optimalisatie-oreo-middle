#pragma once

namespace Tmpl8 
{
class Quadtree 
{
public:
    Quadtree() : mroot(std::make_unique<Node>()) {}
    void Quadtree::add(vec2 point) { add(mroot.get(), point, min, max); };

private:
    //array<unique_ptr<Quadtree>, 4> *children1 = nullptr; 
    //Divide quadtree in 4 other Quadtree
    struct Node {
        array<unique_ptr<Node>, 4> children;
        vec2 node_point = (0, 0);
        bool leafnode = true; // Quadtree is a leafnode when there is max 1 point
        bool empty = true;
    };

    std::unique_ptr<Node> mroot;

    // min x,y and max x,y values of Quadtree
    vec2 min = (0,0);
    vec2 max = (SCRWIDTH, SCRHEIGHT);

    int Quadtree::getQuadrant(vec2& p, vec2 min, vec2 max);
    void Quadtree::add(Node* node, vec2& p, vec2 min, vec2 max);
    void Quadtree::split(Node* node, vec2 min, vec2 max);
};

}


