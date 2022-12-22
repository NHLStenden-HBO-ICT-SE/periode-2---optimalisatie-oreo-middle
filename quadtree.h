#pragma once

namespace Tmpl8 
{
class Quadtree 
{
public:
    Quadtree() : mroot(std::make_unique<Node>()) {}
    void Quadtree::add(vec2 point) { add(mroot.get(), point, min, max, 0); };
    vector<vec2> Quadtree::search(vec2 point) { return search(mroot.get(), point, min, max); };
    void Quadtree::clear() { clear(mroot.get()); };

private:
    //array<unique_ptr<Quadtree>, 4> *children1 = nullptr; 
    //Divide quadtree in 4 other Quadtree
    struct Node {
        array<unique_ptr<Node>, 4> children;
        vector<vec2> points;
        bool leafnode = true;
        bool empty = true;
    };

    std::unique_ptr<Node> mroot;

    // min x,y and max x,y values of Quadtree
    vec2 min = (0,0);
    vec2 max = (SCRWIDTH, SCRHEIGHT);

    static const int Threshold = 16;
    static const int MaxDepth = 8;

    int Quadtree::getQuadrant(vec2& p, vec2 min, vec2 max);
    void Quadtree::add(Node* node, vec2& p, vec2 min, vec2 max, int depth);
    void Quadtree::split(Node* node, vec2 min, vec2 max);
    vector<vec2> Quadtree::search(Node* node, vec2& p, vec2 min, vec2 max);
    void Quadtree::clear(Node* node);
};

}


