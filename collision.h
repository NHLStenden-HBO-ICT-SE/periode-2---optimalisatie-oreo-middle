#pragma once
namespace Tmpl8
{

class Collision
{
private:
    int cellwidth = 5, cellheight = 5;
    size_t gridRowCount = floor(SCRHEIGHT / cellwidth);
    size_t gridCollCount = floor(SCRWIDTH / cellheight);

    point gridCell = {};
    point current_gridCell = {};
    point target_gridCell = {};
    vec2 gridrange = {};
    
    vector<vector<Tank*>> col;
    vector<vector<vector<Tank*>>> grid;

    vector<Tank*> otherTanks;
    std::mutex collision_mtx;

public:
    Collision();
    ~Collision();

    void insertTank(vec2& tankpos, Tank* tank);
    void removeTank(vec2& tankpos, Tank* tank);
    vector<Tank*> tankCollisionWithTank(vec2& tankpos);
    Tank* rocketCollisionWithTank(Rocket& currentRocket, vector<Tank*>& tanks);
    vector<Tank*> tankCollisionWithParticleBeam(Particle_beam currentBeam);

    void clear();
    void updatePos(vec2& tank_targetPos, vec2& tank_currentPos, Tank* tank);
};

}


