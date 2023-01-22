#pragma once
namespace Tmpl8
{

class Collision
{
private:
    int cellwidth = 7, cellheight = 7;
    size_t gridRowCount = floor(SCRHEIGHT / cellwidth);
    size_t gridCollCount = floor(SCRWIDTH / cellheight);

    point gridCell = {};
    vec2 gridrange = {};
    
    vector<vector<int>> col;
    vector<vector<vector<int>>> grid;


public:
    Collision();
    ~Collision();

    void insertTank(vec2& tankpos, int tankIndex);
    vector<int> tankCollisionWithTank(Tank& currentTank);
    int rocketCollisionWithTank(Rocket& currentRocket, vector<Tank>* tanks);
    vector<int> tankCollisionWithParticleBeam(Particle_beam currentBeam);

    void clear();
};

}


