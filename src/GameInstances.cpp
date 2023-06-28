#include "GameInstances.h"

// ============= GameInstance methods/functions ====================

// Getters
float GameInstance::get_x_pos() const
{
    return this->x_pos;
}

float GameInstance::get_y_pos() const
{
    return this->y_pos;
}

float GameInstance::get_x_velocity() const
{
    return this->x_velocity;
}

float GameInstance::get_y_velocity() const
{
    return this->y_velocity;
}

float GameInstance::get_radius() const
{
    return this->radius;
}

glm::vec3 GameInstance::get_color() const
{
    return this->color;
}

glm::vec2 GameInstance::get_glm_pos_2d() const
{
    return glm::vec2(this->x_pos, this->y_pos);
}

// Setters
void GameInstance::set_x_pos(float xPos)
{
    this->x_pos = xPos;
}

void GameInstance::set_y_pos(float yPos)
{
    this->y_pos = yPos;
}

void GameInstance::set_x_velocity(float xVel)
{
    this->x_velocity = xVel;
}

void GameInstance::set_y_velocity(float yVel)
{
    this->y_velocity = yVel;
}

void GameInstance::set_radius(float radius)
{
    this->radius = radius;
}

void GameInstance::set_color(const glm::vec3& color)
{
    this->color = color;
}

void GameInstance::gl_render_instance()
{
    // Game Instance object is a circle
    int n = 32; // sides of circles (polygon)
    float d_theta = 360.0f / n;  // degrees per vertice
    float theta = 0.0f;
    glm::vec3 color = this->color;

    // d_theta degrees to radians
    d_theta = 3.141529 * d_theta / 180;

    glColor3f(color.x, color.y, color.z);
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < n; ++i)
    {
        theta = (d_theta * i);
        float x1 = (this->radius * cos(theta)) + this->x_pos;
        float y1 = (this->radius * sin(theta)) + this->y_pos;
        float x2 = (this->radius * cos(theta + d_theta)) + this->x_pos;
        float y2 = (this->radius * sin(theta + d_theta)) + this->y_pos;
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(this->x_pos, this->y_pos);
    }
    glEnd();
}

void GameInstance::update_position(float dt)
{

}

// ======== Player methods definition ===========
// Definition of Player constructor
Player::Player(float x_pos, float y_pos, glm::vec3 color)
{
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    this->x_velocity = 0.0;
    this->y_velocity = 0.0;
    this->slow_down = false;
    this->color = color;
}

// constructor by default
Player::Player()
{
    // later make player settings load from
    // configuration file for constructor by default

    //
    this->radius=0.03;
    this->x_pos = 0.0;
    this->y_pos = 0.0;
    this->x_velocity = 0.0;
    this->y_velocity = 0.0;
    this->slow_down = false;
    this->color = glm::vec3(1.0, 0.0, 0.0);  // red player by default
}

// Definition of Player destructor
Player::~Player()
{
    // Implement the destructor here
}

void Player::update_position(float dt)
{
    float step = 1;

    if (this->slow_down)
        step *= 0.5;

    float x_new = this->x_pos + step * this->dx * dt;
    float y_new = this->y_pos + step * this->dy * dt;

    //std::cout << dx << ',' << dy << '\n';

    if (fabs(x_new) <= 1)
        this->x_pos = x_new;
    if (fabs(y_new) <= 1)
        this->y_pos = y_new;

    //this->reset_direction();
}

void Player::reset_direction()
{
    this->dx = 0.0;
    this->dy = 0.0;
}

void Player::add_direction(float dx, float dy)
{
    // allow diagonal directions
    float dx_new = 0.0, dy_new = 0.0;

    dx_new = this->dx + dx;
    dy_new = this->dy + dy;

    //std::cout << dx_new << ',' << dy_new  << '\n';

    if (fabs(dx_new) <= 1)
        this->dx = dx_new;
    if (fabs(dy_new) <= 1)
        this->dy = dy_new;
}

void Player::set_slow_down(bool slow_down)
{
    this->slow_down = slow_down;
}

void Player::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GameWindow* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    // =========== player motion buttons (a-w-s-d)
    // add speed in particular direction (vertical and horizontal directions)
    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
    {
        // direction
        float dx = -1.0, dy = 0.0;
        this->add_direction(dx, dy);
    }

    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
    {
        float dx = 1.0, dy = 0.0;
        this->add_direction(dx, dy);
    }

    if (key == GLFW_KEY_UP && action != GLFW_RELEASE)
    {
        float dx = 0.0, dy = 1.0;
        this->add_direction(dx, dy);
    }

    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE)
    {
        float dx = 0.0, dy = -1.0;
        this->add_direction(dx, dy);
    }
    // ========= negate motion operations
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
        // direction
        float dx = -1.0, dy = 0.0;
        this->add_direction(-dx, -dy);
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    {
        float dx = 1.0, dy = 0.0;
        this->add_direction(-dx, -dy);
    }

    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
    {
        float dx = 0.0, dy = 1.0;
        this->add_direction(-dx, -dy);
    }

    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    {
        float dx = 0.0, dy = -1.0;
        this->add_direction(-dx, -dy);
    }
    // ==== slow down player
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        this->set_slow_down(true);

    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
        this->set_slow_down(false);

    // player abilities buttons
}

void Player::staticKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Player* player = static_cast<Player*>(glfwGetWindowUserPointer(window));
    player->keyCallback(window, key, scancode, action, mods);
}

void Player::registerKeyCallback(GLFWwindow* window)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, Player::staticKeyCallback);
}

// ========== Enemy methods definition =========
// Definition of Enemy constructor
Enemy::Enemy(float x_pos, float y_pos, float x_velocity, float y_velocity, glm::vec3 color)
{
    this->radius=0.03;
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    this->x_velocity = x_velocity;
    this->y_velocity = y_velocity;
    this->color = color;
}

// Definition of Enemy destructor
Enemy::~Enemy()
{
    // Implement the destructor here
}

void Enemy::update_position(float dt)
{
    float new_xpos = this->x_pos + this->x_velocity * dt;
    float new_ypos = this->y_pos + this->y_velocity * dt;

    if (fabs(new_xpos) < 1)
        this->x_pos = new_xpos;

    if (fabs(new_ypos) < 1)
        this->y_pos = new_ypos;
}

// ============== EnemyPool (singly linked list) functions / methods ==============

EnemyPool::EnemyPool(int difficulty, Player* player_ptr)
{
    // creating enemy pool
    this->enemies = std::vector<Enemy*>();
    unsigned short int n_size = (difficulty + 1) * 8;

    while (this->enemies.size() < n_size)
    {
        Enemy* enemy_ptr = this->enemy_generate();
        this->add_enemy(enemy_ptr, player_ptr);
    }
}

EnemyPool::EnemyPool(unsigned short int n_size, Player* player_ptr)
{
    // creating enemy pool
    this->enemies = std::vector<Enemy*>();

    while (this->enemies.size() < n_size)
    {
        Enemy* enemy_ptr = this->enemy_generate();
        this->add_enemy(enemy_ptr, player_ptr);
    }
}

EnemyPool::EnemyPool()
{
    this->enemies = std::vector<Enemy*>();
}

EnemyPool::~EnemyPool()
{

}

Enemy* EnemyPool::enemy_generate()
{
    // random radius
    float minRadius = 0.025f;
    float maxRadius = 0.05f;
    float randomRadius = minRadius + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxRadius - minRadius)));

    // random position
    float minXPos = -1.0f + randomRadius;
    float maxXPos = 1.0f - randomRadius;
    float minYPos = -1.0f + randomRadius ;
    float maxYPos = 1.0f - randomRadius;
    float randomXPos = minXPos + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxXPos - minXPos)));
    float randomYPos = minYPos + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxYPos - minYPos)));

    // random velocities
    float max_speed = 0.5f;
    float minXVelocity = -max_speed;
    float maxXVelocity = max_speed;
    float minYVelocity = -max_speed;
    float maxYVelocity = max_speed;
    float randomXVelocity = minXVelocity + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxXVelocity - minXVelocity)));
    float randomYVelocity = minYVelocity + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxYVelocity - minYVelocity)));

    Enemy* enemy = new Enemy(randomXPos, randomYPos, randomXVelocity, randomYVelocity, glm::vec3(1.0f, 1.0f, 1.0f));
    enemy->set_radius(randomRadius);
    return enemy;
}

// updating enemy pool elements
void EnemyPool::add_enemy(Enemy* enemy_ptr, Player* player_ptr)
{
    // enemy pool is empty
    if (this->enemies.size() == 0)
        this->enemies.push_back(enemy_ptr);

    // new enemy must not touch the player if created
    float window = 0.2; // min distance between points

    float p_x = player_ptr->get_x_pos();
    float p_y = player_ptr->get_y_pos();
    float p_radius = player_ptr->get_radius();

    float en_x = enemy_ptr->get_x_pos();
    float en_y = enemy_ptr->get_y_pos();
    float en_radius = enemy_ptr->get_radius();

    float player_enemy_dist = distance(p_x, p_y, en_x, en_y);

    // player and enemy do not touch
    bool no_touch = (player_enemy_dist > p_radius + en_radius + window) ? true : false;
    if (!no_touch)
        return;

    // new enemy must not touch other enemies in pool if created
    std::vector<Enemy*> enemy_ptrs = this->get_all_enemies_ptr();
    for (Enemy* enemy_ptr_loop : enemy_ptrs)
    {
        float x1 = enemy_ptr_loop->get_x_pos();
        float y1 = enemy_ptr_loop->get_y_pos();
        float r1 = enemy_ptr_loop->get_radius();
        float dist = distance(x1, y1, en_x, en_y);
        bool no_touch = (dist > r1 + en_radius + window) ? true : false;
        if (!no_touch)
            return;
    }
    this->enemies.push_back(enemy_ptr);
}

void EnemyPool::remove_enemy(unsigned short int index)
{
    // Index is out of range, handle the error or return if desired
    if (index >= enemies.size())
        return;

    enemies.erase(enemies.begin() + index);
}

std::vector<Enemy*> EnemyPool::get_all_enemies_ptr() const
{
    return this->enemies;
}

// EnemyPool events
bool EnemyPool::player_get_hit(Player* player_ptr) const
{
    // player params
    float x1 = player_ptr->get_x_pos();
    float y1 = player_ptr->get_y_pos();
    float r1 = player_ptr->get_radius();

    for(Enemy* enemy_ptr : this->enemies)
    {
        float x2 = enemy_ptr->get_x_pos();
        float y2 = enemy_ptr->get_y_pos();
        float r2 = enemy_ptr->get_radius();

        float dist = distance(x1, y1, x2, y2);
        if (dist <= r1 + r2)
            return true;
    }

    return false;
}

void EnemyPool::update_enemies_state(float dt)
{
    // recalculating positions
    for(Enemy* enemy_ptr : this->enemies)
    {
        enemy_ptr->update_position(dt);
        float x1 = enemy_ptr->get_x_pos();
        float y1 = enemy_ptr->get_y_pos();
        float r1 = enemy_ptr->get_radius();
        float dx_1 = enemy_ptr->get_x_velocity();
        float dy_1 = enemy_ptr->get_y_velocity();
        if (!(fabs(x1) < 1 - r1 - 0.01f))
            enemy_ptr->set_x_velocity(-dx_1);
        if (!(fabs(y1) < 1 - r1 - 0.01f))
            enemy_ptr->set_y_velocity(-dy_1);

        // handle circles collision
        for (Enemy* other_enemy_ptr : this->enemies)
        {
            if (enemy_ptr != other_enemy_ptr) // ignore self-collision
            {
                float x2 = other_enemy_ptr->get_x_pos();
                float y2 = other_enemy_ptr->get_y_pos();
                float r2 = other_enemy_ptr->get_radius();

                float distance_squared = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
                float combined_radius = r1 + r2;
                // circles overlap
                if (distance_squared < combined_radius * combined_radius)
                {
                    // velocities of another circle
                    float dx_2 = other_enemy_ptr->get_x_velocity();
                    float dy_2 = other_enemy_ptr->get_y_velocity();

                    // vector "between" circles
                    float dx = x2 - x1;
                    float dy = y2 - y1;

                    // Compute the normalized collision normal
                    float n_norm = std::sqrt(dx * dx + dy * dy);
                    float nx = dx / n_norm;
                    float ny = dy / n_norm;

                    // Compute the relative velocity
                    float dvx = dx_1 - dx_2;
                    float dvy = dy_1 - dy_2;

                    // Compute the separation vector
                    float separation = combined_radius - std::sqrt(distance_squared);
                    float sx = separation * nx;
                    float sy = separation * ny;

                    // Apply separation to the positions
                    enemy_ptr->set_x_pos(x1 - 0.5f * sx);
                    enemy_ptr->set_y_pos(y1 - 0.5f * sy);
                    other_enemy_ptr->set_x_pos(x2 + 0.5f * sx);
                    other_enemy_ptr->set_y_pos(y2 + 0.5f * sy);

                    // Reflect velocities along the collision normal
                    float dot_product = dx_1 * nx + dy_1 * ny;
                    float rx = -2.0f * dot_product * nx;
                    float ry = -2.0f * dot_product * ny;

                    enemy_ptr->set_x_velocity(dx_1 + rx);
                    enemy_ptr->set_y_velocity(dy_1 + ry);

                    dot_product = dx_2 * nx + dy_2 * ny;
                    rx = -2.0f * dot_product * nx;
                    ry = -2.0f * dot_product * ny;

                    other_enemy_ptr->set_x_velocity(dx_2 + rx);
                    other_enemy_ptr->set_y_velocity(dy_2 + ry);
                    // Shift one of the circles in the direction of its velocity
                    float shift = 0.01f;
                    enemy_ptr->set_x_pos(enemy_ptr->get_x_pos() + enemy_ptr->get_x_velocity() * shift);
                    enemy_ptr->set_y_pos(enemy_ptr->get_y_pos() + enemy_ptr->get_y_velocity() * shift);
                }
            }
        }
        // +++ handle circles after collision
    }
}

// draw functions
void EnemyPool::gl_render_all()
{
    for(Enemy* enemy_ptr : this->enemies)
        enemy_ptr->gl_render_instance();
}

// special getters

std::vector<std::pair<glm::vec2, glm::vec2>> EnemyPool::get_collision_points_and_curvatures() const
{
    // collision data (collision_point, circles_curvatures)
    std::vector<std::pair<glm::vec2, glm::vec2>> collision_data;

    for (Enemy* enemy_ptr_1 : enemies)
    {
        glm::vec2 p1 = enemy_ptr_1->get_glm_pos_2d();
        float r1 = enemy_ptr_1->get_radius();

        for (Enemy* enemy_ptr_2 : enemies)
        {
            if (enemy_ptr_1 == enemy_ptr_2)
                continue;

            glm::vec2 p2 = enemy_ptr_2->get_glm_pos_2d();
            float r2 = enemy_ptr_2->get_radius();

            // Calculate the distance between the centers
            float distance = glm::distance(p1, p2);

            // Check if circles collide
            if (distance < r1 + r2 + COLLISION_WINDOW)
            {
                // Calculate the intersection points
                float d = distance;
                float a = (r1 * r1 - r2 * r2 + d * d) / (2 * d); // from intersection to the 1st point
                float h = sqrt(r1 * r1 - a * a);  // distance to p1-p2 line from intersection

                // Midpoint between the centers
                glm::vec2 midpoint = p1 + a * (p2 - p1) / d;

                // Intersection point
                glm::vec2 intersection = midpoint + h * glm::normalize(glm::vec2(p2.y - p1.y, p1.x - p2.x));

                // Add the collision point and curvature to the collision data
                collision_data.push_back(std::make_pair(intersection, glm::vec2(r1, r2)));

                // Only one intersection point per pair
            }
        }
    }

    return collision_data;
}

std::vector<float> EnemyPool::get_collision_slopes() const
{
    std::vector<float> collision_slopes = std::vector<float>();

    for (Enemy* enemy_ptr_1 : enemies)
    {
        glm::vec2 p1 = enemy_ptr_1->get_glm_pos_2d();
        float r1 = enemy_ptr_1->get_radius();

        for (Enemy* enemy_ptr_2 : enemies)
        {
            if (enemy_ptr_1 == enemy_ptr_2)
                continue;

            glm::vec2 p2 = enemy_ptr_2->get_glm_pos_2d();
            float r2 = enemy_ptr_2->get_radius();

            // Calculate the distance between the centers
            float distance = glm::distance(p1, p2);

            // Check if circles collide
            if (distance < r1 + r2 + COLLISION_WINDOW)
            {
                glm::vec2 slope_vec = p2 - p1;
                float angle = glm::degrees( glm::angle(slope_vec, glm::vec2(1.0f, 0.0f)) );
                collision_slopes.push_back(angle);
                // one collision per pair
            }
        }
    }
    return collision_slopes;
}

// ===== Helper function implementation =============================
float vector_norm(float x, float y)
{
    return sqrt(x * x + y * y);
}

float distance(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1, dy = y2 - y1;
    return vector_norm(dx, dy);
}


