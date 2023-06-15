#ifndef ANIMATIONS_H_INCLUDED
#define ANIMATIONS_H_INCLUDED

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>

#include "GameInstances.h"

// ======= forward declarations
class EnemyPool;
class Player;

// ============ Shapes in 2D ===================

class Shape
{
protected:

public:
    virtual void gl_render(float dt); // ??
};


class Polygon
{
private:
public:
};

// ========= Particles (mostly for collision animations) ======
class Particles
{
private:
    std::vector<glm::vec2> particles_around_center;
    float x_pos;
    float y_pos;
    glm::vec4 color; // rgba color
    float curvature_1;
    float curvature_2;
    float theta;
    float time_span; // life time of the particles
    float time_elapsed;

public:
    Particles(float x_pos, float y_pos, float curvature_1, float curvature_2, float theta);
    Particles(); // default constructor
    ~Particles();

    // Add other member functions and variables as needed
    void update_positions(float dt);
    void gl_render() const;

    float get_time_span() const;
    float get_time_elapsed() const;
};


// ====================
class Animation
{
private:
    // some game state parameters here
    EnemyPool* enemy_pool_ptr;
    Player* player_ptr;
    std::vector<Particles*> collision_particles_alive;

public:
    Animation(EnemyPool* enemy_pool_ptr, Player* player);
    Animation();
    ~Animation();
    // track game events methods

    // particles animation events
    void particles_alive_pool_update(); // check all particles lifespan exceeded

    //
    void animation_update_state(float dt);

    // render functions
    void gl_animation_render() const;
    void particles_render_collisions() const;
};

#endif // ANIMATIONS_H_INCLUDED
