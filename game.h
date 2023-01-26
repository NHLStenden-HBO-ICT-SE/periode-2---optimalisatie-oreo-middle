#pragma once
#include <set>

namespace Tmpl8
{
//forward declarations
class Tank;
class Rocket;
class Smoke;
class Particle_beam;
class ConvexHull;
class Collision;

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

    vec2& find_closest_enemy(Tank& tank);

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

  private:
    Surface* screen;

    vector<Tank> tanks;
    vector<Tank*> tanks_alive;
    vector<Rocket> rockets;
    vector<Smoke> smokes;
    vector<Explosion> explosions;
    vector<Particle_beam> particle_beams;

    Terrain background_terrain;
    std::vector<vec2> forcefield_hull;
    std::vector<vec2> convex_hull;

    vector<vec2> redTankposlist;
    vector<vec2> blueTankposlist;

    Font* frame_count_font;
    long long frame_count = 0;

    bool lock_update = false;

    //Checks if a point lies on the left of an arbitrary angled line
    bool left_of_line(vec2 line_start, vec2 line_end, vec2 point);

    std::unique_ptr<Quadtree> qtBlue = make_unique<Quadtree>();
    std::unique_ptr<Quadtree> qtRed = make_unique<Quadtree>();

    std::unique_ptr<Collision> grid = make_unique<Collision>();
};



}; // namespace Tmpl8