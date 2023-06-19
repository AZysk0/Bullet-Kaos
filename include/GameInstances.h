#ifndef GAMEINSTANCES_H
#define GAMEINSTANCES_H
#define COLLISION_WINDOW 0.0f
// graphics libraries
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
//
#include <iostream>
#include <vector>
#include <cmath>

#include "GameWindow.h"


// =========== Helper functions ===================
float vector_norm(float x, float y);
float distance(float x1, float y1, float x2, float y2);

// ================================================
class GameInstance
{
protected:
    float x_pos;
    float y_pos;
    float x_velocity;
    float y_velocity;
    float radius;
    glm::vec3 color;
public:
    virtual void gl_render_instance();
    virtual void update_position(float dt);

    // Getters
    float get_x_pos() const;
    float get_y_pos() const;
    float get_x_velocity() const;
    float get_y_velocity() const;
    float get_radius() const;
    glm::vec3 get_color() const;
    glm::vec2 get_glm_pos_2d() const;

    // Setters
    void set_x_pos(float xPos);
    void set_y_pos(float yPos);
    void set_x_velocity(float xVel);
    void set_y_velocity(float yVel);
    void set_radius(float radius);
    void set_color(const glm::vec3& color);
};


// ================================================
class Enemy : public GameInstance
{
private:

public:
    Enemy(float x_pos, float y_pos, float x_velocity, float y_velocity, glm::vec3 color);
    Enemy();
    ~Enemy();
    void update_position(float dt) override;
};

// =========== Player =============================
class Player : public GameInstance
{
private:
    float dx;
    float dy;
    bool slow_down;

public:
    Player(float x_pos, float y_pos, glm::vec3 color);
    Player();
    ~Player();
    //
    void load_player_config();
    //
    void update_position(float dt) override;
    void reset_direction();
    //
    void add_direction(float dx, float dy);
    void set_slow_down(bool);

    // key callback functions
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods); // static wrapper
    void registerKeyCallback(GLFWwindow* window); // register key
};


// ============= EnemyPool Singly Linked List ================= //
class EnemyPool
{
private:
    std::vector<Enemy*> enemies;

public:
    EnemyPool(unsigned short int n_size, Player* player_ptr);
    EnemyPool();
    ~EnemyPool();
    //
    Enemy* enemy_generate();
    void add_enemy(Enemy* enemy_ptr, Player* player_ptr);
    void remove_enemy(unsigned short int index);
    std::vector<Enemy*> get_all_enemies_ptr() const;

    // event functions
    bool player_get_hit(Player* player_ptr) const;
    void update_enemies_state(float dt);
    // special getters
    std::vector<std::pair<glm::vec2, glm::vec2>> get_collision_points_and_curvatures() const;
    std::vector<float> get_collision_slopes() const;

    // draw functions
    void gl_render_all();
};

// std::vector<Enemy*> create_enemy_pool(unsigned short int n_size, Player* player_ptr);


#endif // GAMEINSTANCES_H_INCLUDED
