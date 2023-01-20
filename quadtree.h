#pragma once

namespace Tmpl8 
{
class Quadtree 
{
public:
    Quadtree() : root(std::make_unique<Node>()), boundries(std::make_unique<Boundries>()){}
    void Quadtree::add(vec2 point) { add(root.get(), point, boundries.get(), 0); };
    vector<vec2> Quadtree::search(vec2 point) { return search(root.get(), point); };
    void Quadtree::clear() { clear(root.get()); };

private:
    struct Boundries {
        int minx = 0;
        int miny = 0;
        int maxx = SCRWIDTH;
        int maxy = SCRHEIGHT;
    };

    struct Node {
        array<unique_ptr<Node>, 4> children;
        vector<vec2> points;
        std::unique_ptr<Boundries> boundries;
        bool leafnode = true;
        bool empty = true;
        Node* parent;
    };

    std::unique_ptr<Node> root;
    std::unique_ptr<Boundries> boundries;

    static const int Threshold = 10;
    static const int MaxDepth = 20;

    int Quadtree::getQuadrant(vec2& p, Boundries* boundries);
    void Quadtree::add(Node* node, vec2& p, Boundries* boundries, int depth);
    void Quadtree::split(Node* node);

    vector<vec2> Quadtree::search(Node* node, vec2& p);
    Quadtree::Node* Quadtree::findNeighbourNodes(Node* node, int dir);
    vector<vec2> Quadtree::findNeighbourPoints(Node* node, int dir);

    void Quadtree::clear(Node* node);
    void Quadtree::newBoundries(Boundries* boundries, int q);
};

}