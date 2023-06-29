#include "precomp.h"
#include "collision.h"
namespace Tmpl8
{

    Collision::Collision() {
        col.resize(gridCollCount);
        grid.resize(gridRowCount, col);
    }

    Collision::~Collision() {}

    void Collision::insertTank(vec2& tankpos, Tank* tank) {
        gridCell.x = floor(tankpos.x / cellwidth);
        gridCell.y = floor(tankpos.y / cellheight);

        grid[gridCell.y][gridCell.x].push_back(tank);
    }

    void Collision::removeTank(vec2& tankpos, Tank* tank) {
        gridCell.x = floor(tankpos.x / cellwidth);
        gridCell.y = floor(tankpos.y / cellheight);

        std::vector<Tank*>& gridCellVector = grid[gridCell.y][gridCell.x];
        auto it = std::find(gridCellVector.begin(), gridCellVector.end(), tank);
        if (it != gridCellVector.end()) {
            gridCellVector.erase(it);
        }
    }

    vector<Tank*> Collision::tankCollisionWithTank(vec2& tankpos) {
        gridCell.x = floor(tankpos.x / cellwidth);
        gridCell.y = floor(tankpos.y / cellheight);

        otherTanks.clear();

        // For every direction checking neighbour grid cells. 9 total
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2;j++) {
                for (Tank* tank : grid[gridCell.y + i][gridCell.x + j]) {
                    otherTanks.push_back(tank);
                }
            }
        }

        return otherTanks;
    }

    Tank* Collision::rocketCollisionWithTank(Rocket& currentRocket, vector<Tank*>& tanks) {
        vec2 rocketpos = currentRocket.get_position();
        gridCell.x = floor(rocketpos.x / cellwidth);
        gridCell.y = floor(rocketpos.y / cellheight);

        otherTanks.clear();

        // For every direction checking neighbour gridcells and adding tankindex to list
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                for (Tank* index : grid[gridCell.y + i][gridCell.x + j]) {
                    otherTanks.push_back(index);
                }
            }
        }

        // Check collision with every tank of neighbour gridcells and return if collided
        for (Tank* otherTank : otherTanks) {
            if (currentRocket.allignment == otherTank->allignment) continue;
            if (currentRocket.intersects(otherTank->get_position(), otherTank->get_collision_radius())) {
                currentRocket.active = false;
                return otherTank;
            }
        }
        return NULL;
    }

    vector<Tank*> Collision::tankCollisionWithParticleBeam(Particle_beam currentBeam) {
        // Upperleft most point of particel beam
        gridCell.x = floor(currentBeam.min_position.x / cellwidth);
        gridCell.y = floor(currentBeam.min_position.y / cellheight);

        // Range of gridcells according to size of particle beam
        gridrange.x = floor((currentBeam.max_position.x - currentBeam.min_position.x) / cellwidth);
        gridrange.y = floor((currentBeam.max_position.y - currentBeam.min_position.y) / cellwidth);

        vector<Tank*> otherTanks;

        // add tanks of cell to list for every gridcell inrange of particle beam
        for (int i = 0; i < gridrange.y; i++) {
            for (int j = 0; j < gridrange.x; j++) {
                for (Tank* tank : grid[gridCell.y + i][gridCell.x + j]) {
                    otherTanks.push_back(tank);
                }
            }
        }

        return otherTanks;
    }

    void Collision::clear() {
        for (int i = 0; i < gridRowCount; i++) {
            for (int j = 0; j < gridCollCount; j++) {
                grid[i][j].clear();
            }
        }
    };

    void Collision::updatePos(vec2& tank_targetPos, vec2& tank_currentPos, Tank* tank) {
        target_gridCell.x = floor(tank_targetPos.x / cellwidth);
        target_gridCell.y = floor(tank_targetPos.y / cellheight);

        current_gridCell.x = floor(tank_currentPos.x / cellwidth);
        current_gridCell.y = floor(tank_currentPos.y / cellheight);

        if (target_gridCell.x != current_gridCell.x || target_gridCell.y != current_gridCell.y) {
            removeTank(tank_currentPos, tank);
            insertTank(tank_targetPos, tank);
        }
    }
}