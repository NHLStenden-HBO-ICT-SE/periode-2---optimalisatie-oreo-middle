#pragma once
namespace Tmpl8
{

class Collision
{
private:
    int cellwidth = 7, cellheight = 7;
    size_t gridRowCount = floor(SCRHEIGHT / cellwidth);
    size_t gridCollCount = floor(SCRWIDTH / cellheight);

    point gridCell;

    struct cell
    {
        vector<int> tankindexes;
        vector<int> rocketindexes;
    };
    
    vector<cell> col;
    vector<vector<cell>> grid;

public:
    Collision();
    ~Collision();

    void insert(vec2& tankpos, int tankIndex);
    void tankCollisionWithTank(Tank& currentTank, vector<Tank>* tanks);
    int rocketCollisionWithTank(Rocket& currentRocket, vector<Tank>* tanks);

    void clear() { grid.clear(); col.resize(gridCollCount);
                                grid.resize(gridRowCount, col);
    };
};

}


