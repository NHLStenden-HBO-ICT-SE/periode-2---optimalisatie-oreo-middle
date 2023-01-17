#include "precomp.h"
#include "collision.h"
namespace Tmpl8
{
    Collision::Collision() {
        clear();
    }

    Collision::~Collision() {}

    void Collision::insert(vec2& tankpos, int tankIndex) {
        gridCell.x = floor(tankpos.x / cellwidth);
        gridCell.y = floor(tankpos.y / cellheight);

        grid[gridCell.y][gridCell.x].tankindexes.push_back(tankIndex);
    }

    void Collision::tankCollisionWithTank(Tank& currentTank, vector<Tank>* tanks) {
        vec2 tankpos = currentTank.get_position();
        gridCell.x = floor(tankpos.x / cellwidth);
        gridCell.y = floor(tankpos.y / cellheight);

        vector<int> otherTankindexes;

        // For every direction checking neighbour grid cells. 9 total
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2;j++) {
                for (int& index : grid[gridCell.y + i][gridCell.x + j].tankindexes) {
                    otherTankindexes.push_back(index);
                }
            }
        }

        for (int& otherTankindex : otherTankindexes) {
            if (&currentTank == &(tanks->at(otherTankindex))) continue;

            vec2 tankpos = currentTank.get_position();

            vec2 otherTankpos = tanks->at(otherTankindex).get_position();

            vec2 dir = tankpos - otherTankpos;
            float dir_squared_len = dir.sqr_length();

            float col_squared_len = (currentTank.collision_radius + currentTank.collision_radius);
            col_squared_len *= col_squared_len;

            if (dir_squared_len < col_squared_len)
            {
                currentTank.push(dir.normalized(), 1.f);
            }
        }
    }

    int Collision::rocketCollisionWithTank(Rocket& currentRocket, vector<Tank>* tanks) {
        vec2 rocketpos = currentRocket.get_position();
        gridCell.x = floor(rocketpos.x / cellwidth);
        gridCell.y = floor(rocketpos.y / cellheight);

        vector<int> otherTankindexes;

        // For every direction checking neighbour grid cells. 9 total
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                for (int& index : grid[gridCell.y + i][gridCell.x + j].tankindexes) {
                    otherTankindexes.push_back(index);
                }
            }
        }

        for (int& otherTankindex : otherTankindexes) {
            if (currentRocket.allignment == tanks->at(otherTankindex).allignment) continue;
            if (currentRocket.intersects(tanks->at(otherTankindex).get_position(), tanks->at(otherTankindex).get_collision_radius())) {
                currentRocket.active = false;
                return otherTankindex;
            }
        }
        return -1;
    }
}