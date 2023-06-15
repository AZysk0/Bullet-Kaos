#include "animations.h"
#include "GameInstances.h"

// ========== Shapes methods ==============================


// ========== Particles methods definition ================
// Constructor with parameters
Particles::Particles(float x_pos, float y_pos, float curvature_1, float curvature_2, float theta)
{
    float alpha = 0.0f;
    // Initialize member variables
    this->color = glm::vec4(1.0f, 0.5f, 0.0f, alpha); // transparent
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    this->curvature_1 = curvature_1;
    this->curvature_2 = curvature_2;
    this->theta = theta;
    this->time_span = 0.6f;
    this->time_elapsed = 0.0f;

    // Initialize particles_pos vector
    float neighborhood_size = 0.03f; // Adjust the size of the neighborhood as needed
    int num_particles = 50; // Adjust the number of particles as needed

    this->particles_around_center.reserve(num_particles); // reserve for efficient insertion

    // generating random particles in some neighborhood
    for (int i = 0; i < num_particles; ++i)
    {
        float offset_x = ((rand() % 100) / 100.0) * neighborhood_size - neighborhood_size / 2.0f;
        float offset_y = ((rand() % 100) / 100.0) * neighborhood_size - neighborhood_size / 2.0f;

        glm::vec2 particle_position(offset_x, offset_y);
        this->particles_around_center.push_back(particle_position);
    }
}

// Default constructor
Particles::Particles()
{
    // Initialize member variables with default values
    // ...
}

// Destructor
Particles::~Particles()
{
    // Clean up resources, if needed
    // ...
}

void Particles::update_positions(float dt)
{
    // solving Cauchy problem y' = (y^2 - x^2) / (2xy) at point
    // find first iteration solution for each position
    float c1 = this->curvature_1;
    float c2 = this->curvature_2;
    float gamma_ = 10;
    // later use curvatures to fit the hitting circles
    float step = gamma_ * c1 * c1;
    float step_reverse = gamma_ * c2 *c2;

    bool reverse_ = false; // determine whether to reverse the update direction
    for (glm::vec2& particle : this->particles_around_center) // reference to ensure update of the points
    {
        //
        if (reverse_)
        {
            float x0 = particle.x;
            float y0 = particle.y;

            particle.x += -dt * step_reverse;
            particle.y += (-dt * step_reverse) * (y0 * y0 - x0 * x0) / (2 * x0 * y0);
        }
        else
        {
            float x0 = particle.x;
            float y0 = particle.y;

            particle.x += (dt * step);
            particle.y += (dt * step) * (y0 * y0 - x0 * x0) / (2 * x0 * y0);
        }
        reverse_ = !reverse_; // åoggle the reverse flag
    }
    this->time_elapsed += dt;
}

// Render particles
void Particles::gl_render() const
{
    // Calculate the progress based on the time elapsed and the total time span
    float t1 = this->time_elapsed, t = this->time_span;
    float progress = t1 / t;

    // Clamp the progress between 0 and 1 to ensure it stays within the valid range
    progress = glm::clamp(progress, 0.0f, 1.0f);

    // Interpolate between white color and `this->color` based on the progress
    glm::vec4 current_color = glm::mix(glm::vec4(1.0f), this->color, progress);

    // transform space for drawing particles around collision point
    std::cout << this->theta << '\n';
    glPushMatrix(); // Save the current transformation state

    glTranslatef(this->x_pos, this->y_pos, 0.0);
    glRotatef(this->theta, 0.0f, 0.0f, 1.0f);

    glPointSize(1.0f);
    glColor4f(current_color.r, current_color.g, current_color.b, current_color.a);
    glBegin(GL_POINTS);
    for (glm::vec2 particle : this->particles_around_center)
    {
        glVertex2f(particle.x, particle.y);
    }
    glEnd();
    glPopMatrix();

    glLoadIdentity();
}

float Particles::get_time_span() const
{
    return this->time_span;
}

float Particles::get_time_elapsed() const
{
    return this->time_elapsed;
}


// ====== Animation methods/functions ===============
Animation::Animation(EnemyPool* enemy_pool_ptr, Player* player_ptr)
{
    this->enemy_pool_ptr = enemy_pool_ptr;
    this->player_ptr = player_ptr;
    this->collision_particles_alive = std::vector<Particles*>();
}

Animation::Animation()
{

}

Animation::~Animation()
{

}

void Animation::particles_alive_pool_update()
{
    // Get the collision data (collision points, curvatures, angles)
    std::vector<std::pair<glm::vec2, glm::vec2>> collision_data = this->enemy_pool_ptr->get_collision_points_and_curvatures();
    std::vector<float> angles = this->enemy_pool_ptr->get_collision_slopes();

    // Add new particles at collision points
    for (size_t i = 0; i < collision_data.size(); i++)
    {
        const auto& collision = collision_data[i];
        const glm::vec2& collision_point = collision.first;
        const glm::vec2& curvatures = collision.second;
        float theta = angles[i];

        // Add new particles using the collision point, curvatures, and theta
        Particles* particles_new = new Particles(collision_point.x, collision_point.y, curvatures.x, curvatures.y, theta);
        this->collision_particles_alive.push_back(particles_new);
    }

    if (this->collision_particles_alive.size() == 0)
        return;

    // Remove particles that exceeded time_span and delete them from memory

    // remove iterator
    auto iterator = std::remove_if(
        this->collision_particles_alive.begin(),
        this->collision_particles_alive.end(),
        [](Particles* particles)
        {
            if (particles->get_time_elapsed() > particles->get_time_span()) {
                delete particles;
                return true; // Remove from vector
            }
            return false;
        }
    );

    this->collision_particles_alive.erase(iterator, this->collision_particles_alive.end());
}

void Animation::animation_update_state(float dt)
{
    // remove dead particles, add new
    this->particles_alive_pool_update();

    // update position for each particle
    for(Particles* particles_ptr : this->collision_particles_alive)
    {
        particles_ptr->update_positions(dt);
    }
}

void Animation::particles_render_collisions() const
{
    //std::vector<float> angles = this->enemy_pool_ptr->get_collision_slopes();
    for(Particles* particles_ptr : this->collision_particles_alive)
        particles_ptr->gl_render();
}

void Animation::gl_animation_render() const
{
    // animate collisions
    this->particles_render_collisions();
}
