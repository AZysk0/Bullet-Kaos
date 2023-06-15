#include "GameWindow.h"

GameWindow::~GameWindow()
{

}

bool GameWindow::Initialize()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Create a window
    m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Set callbacks
    glfwSetWindowUserPointer(m_window, this);
    glfwSetKeyCallback(m_window, KeyCallback);
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);

    // Make the window's context current
    glfwMakeContextCurrent(m_window);

    return true;
}

void GameWindow::Run()
{
    while (!glfwWindowShouldClose(m_window))
    {
        // Render here

        // Swap front and back buffers
        glfwSwapBuffers(m_window);

        // Poll for and process events
        glfwPollEvents();
    }
}

void GameWindow::Shutdown()
{
    glfwTerminate();
}

// ================ Game mainloop functions ==========================
void GameWindow::game_setup()
{
    glViewport(0, 0, this->m_width, this->m_height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glfwWindowHint(GLFW_REFRESH_RATE, 60);
}

void GameWindow::game_main_loop()
{
    // =============
    // gameloop consts
    bool game_over = false;
    unsigned short int n_enemies = 40;

    Player* player_ptr = new Player();
    EnemyPool* enemy_pool_ptr = new EnemyPool(n_enemies, player_ptr);
    Animation* animation_ptr = new Animation(enemy_pool_ptr, player_ptr);

    // ==============
    // setup
    player_ptr->registerKeyCallback(this->m_window);
    this->game_setup();

    double t1 = glfwGetTime();

    while (!game_over && !glfwWindowShouldClose(this->m_window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double t2 = glfwGetTime();
        double dt = t2 - t1;

        t1 = t2;
        //std::cout << static_cast<int>(1 / dt) << std::endl;
        player_ptr->update_position(dt);
        animation_ptr->animation_update_state(dt);
        enemy_pool_ptr->update_enemies_state(dt);


        player_ptr->gl_render_instance();
        animation_ptr->gl_animation_render();
        enemy_pool_ptr->gl_render_all();

        if (enemy_pool_ptr->player_get_hit(player_ptr))
            game_over = true;

        glfwSwapBuffers(this->m_window);
        glfwPollEvents();
     }
}

// ================ GLFW events methods/functions ====================
void GameWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GameWindow* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));

//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//    {
//        glfwSetWindowShouldClose(window, true);
//    }
}

void GameWindow::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GameWindow::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    GameWindow* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));

    // Track mouse motion here
    // xpos and ypos represent the new mouse position
}

void GameWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    GameWindow* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));

    // Track mouse button clicks here
    // button indicates which mouse button was clicked
    // action indicates the action (e.g., GLFW_PRESS, GLFW_RELEASE)
}

// ============= OTHER RENDERING FUNCTIONS =================
void time_sleep(float t)
{

}

void gl_draw_text_window_pos(const char* text, int x, int y, glm::vec3 color)
{

}

void gl_draw_text_raster_pos(const char* text, float x, float y, glm::vec3 color)
{

}


