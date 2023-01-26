#include "precomp.h"
#include "collision.h"
namespace Tmpl8
{

    Collision::Collision() {
        col.resize(gridCollCount);
        grid.resize(gridRowCount, col);
    }

    Collision::~Collision() {}

    void Collision::insertTank(vec2& tankpos, int tankIndex) {
        gridCell.x = floor(tankpos.x / cellwidth);
        gridCell.y = floor(tankpos.y / cellheight);

        grid[gridCell.y][gridCell.x].push_back(tankIndex);
    }

    vector<int> Collision::tankCollisionWithTank(vec2& tankpos) {
        gridCell.x = floor(tankpos.x / cellwidth);
        gridCell.y = floor(tankpos.y / cellheight);

        otherTankindexes.clear();

        // For every direction checking neighbour grid cells. 9 total
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2;j++) {
                for (int& index : grid[gridCell.y + i][gridCell.x + j]) {
                    otherTankindexes.push_back(index);
                }
            }
        }

        return otherTankindexes;
    }

    int Collision::rocketCollisionWithTank(Rocket& currentRocket, vector<Tank*>& tanks) {
        vec2 rocketpos = currentRocket.get_position();
        gridCell.x = floor(rocketpos.x / cellwidth);
        gridCell.y = floor(rocketpos.y / cellheight);

        otherTankindexes.clear();

        // For every direction checking neighbour gridcells and adding tankindex to list
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                for (int& index : grid[gridCell.y + i][gridCell.x + j]) {
                    otherTankindexes.push_back(index);
                }
            }
        }

        // Check collision with every tank of neighbour gridcells and return if collided
        for (int& otherTankindex : otherTankindexes) {
            if (currentRocket.allignment == tanks.at(otherTankindex)->allignment) continue;
            if (currentRocket.intersects(tanks.at(otherTankindex)->get_position(), tanks.at(otherTankindex)->get_collision_radius())) {
                currentRocket.active = false;
                return otherTankindex;
            }
        }
        return -1;
    }

    vector<int> Collision::tankCollisionWithParticleBeam(Particle_beam currentBeam) {
        // Upperleft most point of particel beam
        gridCell.x = floor(currentBeam.min_position.x / cellwidth);
        gridCell.y = floor(currentBeam.min_position.y / cellheight);

        // Range of gridcells according to size of particle beam
        gridrange.x = floor((currentBeam.max_position.x - currentBeam.min_position.x) / cellwidth);
        gridrange.y = floor((currentBeam.max_position.y - currentBeam.min_position.y) / cellwidth);

        vector<int> otherTankindexes;

        // add tanks of cell to list for every gridcell inrange of particle beam
        for (int i = 0; i < gridrange.y; i++) {
            for (int j = 0; j < gridrange.x; j++) {
                for (int& index : grid[gridCell.y + i][gridCell.x + j]) {
                    otherTankindexes.push_back(index);
                }
            }
        }

        return otherTankindexes;
    }

    void Collision::clear() {
        for (int i = 0; i < gridRowCount; i++) {
            for (int j = 0; j < gridCollCount; j++) {
                grid[i][j].clear();
            }
        }
    };
}