#pragma once

namespace Tmpl8
{
//forward declarations
class Tank;
class Rocket;
class Smoke;
class Particle_beam;
class ConvexHull;

class Game
{
  public:
    void set_target(Surface* surface) { screen = surface; }
    void init();
    void shutdown();
    void update(float deltaTime);
    void draw();
    void tick(float deltaTime);
    void insertion_sort_tanks_health(const std::vector<Tank>& original, std::vector<const Tank*>& sorted_tanks, int begin, int end);
    void draw_health_bars(const std::vector<const Tank*>& sorted_tanks, const int team);
    void measure_performance();
    void mergeSortInterval(std::vector<float>& vec, int st, int mid, int end);
    void mergeSort(std::vector<float>& vec, int st, int end);
    int binarySearch(int array[], int x, int low, int high);

    Tank& find_closest_enemy(Tank& current_tank);

    void mouse_up(int button)
    { /* implement if you want to detect mouse button presses */
    }

    void mouse_down(int button)
    { /* implement if you want to detect mouse button presses */
    }

    void mouse_move(int x, int y)
    { /* implement if you want to detect mouse movement */
    }

    void key_up(int key)
    { /* implement if you want to handle keys */
    }

    void key_down(int key)
    { /* implement if you want to handle keys */
    }

    vec2* convexHullData(vec2* points, int count, bool closeThePath, int& resultCount);

    vector<vec2>* ConvexHullManaged(vector<vec2> points, bool closeThePath);

  private:
    Surface* screen;

    vector<Tank> tanks;
    vector<Rocket> rockets;
    vector<Smoke> smokes;
    vector<Explosion> explosions;
    vector<Particle_beam> particle_beams;

    Terrain background_terrain;
    std::vector<vec2> forcefield_hull;
    vector<vec2>* R_forcefield_hull;

    Font* frame_count_font;
    long long frame_count = 0;

    bool lock_update = false;

    //Checks if a point lies on the left of an arbitrary angled line
    bool left_of_line(vec2 line_start, vec2 line_end, vec2 point);
    //void tankCollisionWithTank(int row, int col, Tank& tank, int tankIndex);
    void tankCollisionWithTank(vector<int> otherTankIndexes, int currentTankindex);
    void removeAndAddtoGrid(int tankIndex, bool remove);

    int minrow, maxrow, mincol, maxcol;
    point center, topleft, topright, bottomleft, bottomright;
    int index;
};

struct cell 
{
    vector<int> tankindexes;
    vector<int> bulletindexes;
};

}; // namespace Tmpl8