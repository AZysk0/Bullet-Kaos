#include "GameWindow.h"


// ============= SOME HELPER FUNCTIONS =================
// shapes rendering
void gl_draw_bordered_rectangle(float x_pos, float y_pos,
                                float width, float height,
                                glm::vec4 fill_color, glm::vec4 border_color)
{
    // Draw the filled interior rectangle
    glColor4f(fill_color.r, fill_color.g, fill_color.b, fill_color.a);
    glRectf(x_pos, y_pos, x_pos + width, y_pos + height);

    // Draw the border
    glColor4f(border_color.r, border_color.g, border_color.b, border_color.a);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x_pos, y_pos);
    glVertex2f(x_pos + width, y_pos);
    glVertex2f(x_pos + width, y_pos + height);
    glVertex2f(x_pos, y_pos + height);
    glEnd();
}

// text rendering functions
void gl_draw_text_window_pos(const char* text, unsigned int x, unsigned int y, glm::vec4 color)
{
    GLFWwindow* window_ptr = glfwGetCurrentContext();
    // Set the text color
    glColor4f(color.r, color.g, color.b, color.a);

    // Set the window position for rendering
    glfwSetWindowPos(window_ptr, x, y);

    // Render the text character by character
    for (const char* c = text; *c != '\0'; ++c)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void gl_draw_text_raster_pos(const char* text, float x, float y, glm::vec3 color)
{
    // Set the text color
    glColor3f(color.r, color.g, color.b);

    glRasterPos2f(x, y);
    // Render the text character by character
    for (const char* c = text; *c != '\0'; ++c)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
    glColor3f(1, 1, 1);
}

void gl_draw_menu_label_centered(
    const char* text, float x_pos, float y_pos, float width, float height,
    glm::vec4 font_color, glm::vec4 bg_color, glm::vec4 border_color
)
{
    // (x_pos, y_pos) is a top-left corner of rectangle
    // center is x=0, y in (-1; 1)
    // compute rectangle that is centered by x_pos in main menu
    float x_pos_new = x_pos - 0.5 * width;
    float y_pos_new = y_pos;
    gl_draw_bordered_rectangle(x_pos_new, y_pos_new, width, height, bg_color, border_color);
    // compute text coordinates so that it centered inside rectangle
    // gl_draw_text_window_pos(text, ...)
}

// ====== Game Window methods definition =================
GameWindow::GameWindow(int width, int height, const char* title)
{
    // set window parameters
    this->m_width = width;
    this->m_height = height;
    this->m_title = title;
    this->m_window = nullptr;

    // set different window flags values (mb later change to integer instead of multiple booleans)
    this->is_current_endless = false;   //
    this->is_current_bossfight = false; //
    this->is_current_campaign = false;  //
    this->is_current_customize = false; //
    this->is_current_settings = false;  //
    this->is_current_main_menu = false; // main menu is current window
}

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

    // Initialize FreeGLUT
    int argc = 0;
    char** argv = nullptr;
    glutInit(&argc, argv);

    return true;
}

void GameWindow::Run()
{
    // load settings/configuration from the file

    // create MainMenu window
    //MainMenu* main_menu = new MainMenu(); // heap allocation
    MainMenu main_menu = MainMenu();    // stack allocation
    main_menu.menu_register_key_callback(this->m_window);
    //+++ somehow create current window object and swap between them
    this->is_current_main_menu = true;

    while (!glfwWindowShouldClose(m_window))
    {
        // Render here
        main_menu.render_main_menu();

        // render different menus depending on flags

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
        animation_ptr->gl_animation_render_all();
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
    //GameWindow* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    return;
}

// ========== Game window methods implementation ================================
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
    return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
// =============== Menus methods implementation ======================

MainMenu::MainMenu()
{
    // main menu constructor by default
    this->current_label = 0;    // 0 is a Endless mode
    this->menu_choice = -1;     // -1 will be treated as NULL-like value
}

MainMenu::~MainMenu()
{
    // main menu destructor
}

//
void MainMenu::increment_current_label(short int n)
{
    // implement ring structure of main menu
    this->current_label = (this->current_label + n + this->max_labels) % this->max_labels;
}

// main menu rendering methods
void MainMenu::render_menu_background() const
{
    // create some cool animation on bg in main menu (somehow without dt function)
}

void MainMenu::render_game_logo() const
{
    // map some color4f png image texture (or gif)
}

void MainMenu::render_menu_labels() const
{
    float alpha = 1.0f;
    glm::vec4 color_gray = glm::vec4(0.5f, 0.5f, 0.5f, alpha);
    glm::vec4 color_black = glm::vec4(0.0f, 0.0f, 0.0f, alpha);
    glm::vec4 color_white = glm::vec4(1.0f, 1.0f, 1.0f, alpha);
    // depending on current label count render main menu labels (Endless, Bossfight, Campaign, Customize, Settings)

    // assign color to each label
    glm::vec4 color_endless_font = color_white;
    glm::vec4 color_endless_bg = color_gray;
    glm::vec4 color_endless_border = color_white;

    glm::vec4 color_bossfight_font = color_white;
    glm::vec4 color_bossfight_bg = color_gray;
    glm::vec4 color_bossfight_border = color_white;

    glm::vec4 color_campaign_font = color_white;
    glm::vec4 color_campaign_bg = color_gray;
    glm::vec4 color_campaign_border = color_white;

    glm::vec4 color_customize_font = color_white;
    glm::vec4 color_customize_bg = color_gray;
    glm::vec4 color_customize_border = color_white;

    glm::vec4 color_highscores_font = color_white;
    glm::vec4 color_highscores_bg = color_gray;
    glm::vec4 color_highscores_border = color_white;

    glm::vec4 color_settings_font = color_white;
    glm::vec4 color_settings_bg = color_gray;
    glm::vec4 color_settings_border = color_white;

    // change color depending on current main menu label
    switch (this->current_label)
    {
        case 0:
            color_endless_font = color_black;
            color_endless_bg = color_white;
            color_endless_border = color_white;
            break;
        case 1:
            color_bossfight_font = color_black;
            color_bossfight_bg = color_white;
            color_bossfight_border = color_white;
            break;
        case 2:
            color_campaign_font = color_black;
            color_campaign_bg = color_white;
            color_campaign_border = color_white;
            break;
        case 3:
            color_customize_font = color_black;
            color_customize_bg = color_white;
            color_customize_border = color_white;
            break;
        case 4:
            color_highscores_bg = color_white;
            color_highscores_font = color_black;
            color_highscores_border = color_white;
            break;
        case 5:
            color_settings_font = color_black;
            color_settings_bg = color_white;
            color_settings_border = color_white;
            break;
        default:
            break;
    }
    // ===== rendering labels ======
    float label_start_heigh = 0.1f;
    float label_width = 0.6f;
    float label_height = 0.15f;
    float label_spacing = 0.05f;
    float label_height_step = -(label_height + label_spacing);

    // 6 different labels (setup the label top-left corner coordinates)
    // Endless
    gl_draw_menu_label_centered(
        "Endless", 0, label_start_heigh, label_width, label_height,
        color_endless_font, color_endless_bg, color_endless_border
    );
    // Boss-fight
    gl_draw_menu_label_centered(
        "Bossfight", 0,  label_start_heigh + label_height_step, label_width, label_height,
        color_bossfight_font, color_bossfight_bg, color_bossfight_border
    );
    // Campaign
    gl_draw_menu_label_centered(
        "Campaign", 0, label_start_heigh + label_height_step * 2, label_width, label_height,
        color_campaign_font, color_campaign_bg, color_campaign_border
    );
    // Customize
    gl_draw_menu_label_centered(
        "Customize", 0, label_start_heigh + label_height_step * 3, label_width, label_height,
        color_customize_font, color_customize_bg, color_customize_border
    );
    // Highscores
    gl_draw_menu_label_centered(
        "Highscores", 0, label_start_heigh + label_height_step * 4, label_width, label_height,
        color_highscores_font, color_highscores_bg, color_highscores_border
    );
    // Settings
    gl_draw_menu_label_centered(
        "Settings", 0, label_start_heigh + label_height_step * 5, label_width, label_height,
        color_settings_font, color_settings_bg, color_settings_border
    );
}

void MainMenu::render_main_menu() const
{
    // render all components defined earlier
    this->render_menu_background(); // bg first because it must be the "furthest" layer of the rendering
    this->render_game_logo();
    this->render_menu_labels();
}

// setters/getters
void MainMenu::set_current_label(unsigned short int n)
{
    // check if outside of the set of possible values
    this->current_label = n;
    this->current_label %= this->max_labels;    // circular structure of menu
}

void MainMenu::set_menu_choice(unsigned short int n)
{
    bool is_legal = (n > this->max_labels) ? false : true;
    if (is_legal)
        this->current_label = n;
}

unsigned short int MainMenu::get_current_label() const
{
    return this->current_label;
}

unsigned short int MainMenu::get_menu_choice() const
{
    return this->menu_choice;
}

// glfw key callback functions
void MainMenu:: menu_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //MainMenu* mainMenu = static_cast<MainMenu*>(glfwGetWindowUserPointer(window));

    // handle !release key events
    if (action != GLFW_RELEASE) // mb action == GLFW_PRESS (test it out)
    {
        switch (key)
        {
            case GLFW_KEY_UP:
                this->increment_current_label(-1);
                break;
            case GLFW_KEY_DOWN:
                this->increment_current_label(1);
                break;
            default:
                break;
        }
    }
    else if (action == GLFW_PRESS) // handle press events
    {
        switch (key)
        {
            case GLFW_KEY_ENTER:
                // Handle the menu choice or trigger an action
                break;
            default:
                break;
        }
    }
}

void MainMenu::static_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    MainMenu* mainMenu = static_cast<MainMenu*>(glfwGetWindowUserPointer(window));
    mainMenu->menu_key_callback(window, key, scancode, action, mods);
}

void MainMenu::menu_register_key_callback(GLFWwindow* window)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, static_key_callback);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

