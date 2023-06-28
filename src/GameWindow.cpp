#include "GameWindow.h"


// ============= SOME HELPER FUNCTIONS =================
// === shapes rendering
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

// === handling OpenGL metrics
glm::vec2 pixel_to_opengl_coordinates(int px, int py)
{
    GLFWwindow* window = glfwGetCurrentContext();
    int wnd_width, wnd_height;
    glfwGetWindowSize(window, &wnd_width, &wnd_height);

    float gl_x = (2.0f * px / wnd_width) - 1.0f;
    float gl_y = 1.0f - (2.0f * py / wnd_height);

    return glm::vec2(gl_x, gl_y);
}

glm::vec2 opengl_to_pixel_coordinates(float gl_x, float gl_y)
{
    GLFWwindow* window = glfwGetCurrentContext();
    int wnd_width, wnd_height;
    glfwGetWindowSize(window, &wnd_width, &wnd_height);

    int px = static_cast<int>((gl_x + 1.0f) * wnd_width / 2.0f);
    int py = static_cast<int>((1.0f - gl_y) * wnd_height / 2.0f);

    return glm::vec2(px, py);
}

// === text rendering functions
int get_text_bitmap_width(const char* text, void* font)
{
    int length = 0;
    if (text != nullptr && font != nullptr)
    {
        length = glutBitmapLength(font, reinterpret_cast<const unsigned char*>(text));
    }
    return length;
}

int get_text_bitmap_height(void* font)
{
    int height = 0;
    if (font != nullptr)
    {
        height = glutBitmapHeight(font);
    }
    return height;
}

void gl_draw_text_window_pos(const char* text, void* font, float x, float y, glm::vec4 color)
{
    GLFWwindow* window_ptr = glfwGetCurrentContext();
    // Set the text color
    glColor4f(color.r, color.g, color.b, color.a);

    glRasterPos2f(x, y);
    // Render the text character by character
    for (const char* c = text; *c != '\0'; ++c)
    {
        glutBitmapCharacter(font, *c);
    }
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void gl_draw_text_raster_pos(const char* text, void* font, float x, float y, glm::vec3 color)
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
    glm::vec4 font_color, glm::vec4 bg_color, glm::vec4 border_color,
    void* font
)
{
    // GLFW window current parameters
    GLFWwindow* window = glfwGetCurrentContext();
    int wnd_width, wnd_height;
    glfwGetWindowSize(window, &wnd_width, &wnd_height);

    // convert
    int text_width_px = get_text_bitmap_width(text, GLUT_BITMAP_HELVETICA_18);
    int text_height_px = get_text_bitmap_height(GLUT_BITMAP_HELVETICA_18);

    // (x_pos, y_pos) is a top-left corner of rectangle
    // center is x=0, y in (-1; 1)
    // compute rectangle that is centered by x_pos in main menu
    float x_pos_new = x_pos - 0.5 * width;
    float y_pos_new = y_pos;
    gl_draw_bordered_rectangle(x_pos_new, y_pos_new, width, height, bg_color, border_color);

    // compute text coordinates so that it centered inside rectangle
    // gl_draw_text_window_pos(text, ...)
    int text_bitmap_width_px = get_text_bitmap_width(text, font);
    int text_bitmap_height_px = get_text_bitmap_height(font);
    float text_bitmap_width_gl = text_bitmap_width_px / static_cast<float>(wnd_width);
    float text_bitmap_height_gl = text_bitmap_height_px / static_cast<float>(wnd_height);
    // draw text centered inside already drawn rectangle
    float text_x = x_pos_new + 0.5f * (width - text_bitmap_width_gl) - 0.07f;
    float text_y = y_pos_new + 0.5f * (height - text_bitmap_height_gl);
    gl_draw_text_window_pos(text, font, text_x, text_y, font_color);
}

// === game structure choice events processing

template <typename T>
bool is_value_in_vector(const std::vector<T>& vec, const T& target) {
    auto it = std::find(vec.begin(), vec.end(), target);
    return it != vec.end();
}

void process_choice_events(GameStructure* game_structure)
{
    std::string menu_type = game_structure->get_current_menu()->get_menu_name();

    // check if menu is a choice menu
    std::vector<std::string> choice_menu_types  = {"MainMenu", "PauseMenu", "GameoverMenu"};

    if (!is_value_in_vector(choice_menu_types, menu_type)) return;

    // casting to appropriate menu type
    void* choice_menu_object = game_structure->get_current_menu()->get_menu_object_ptr();
    MainMenu* main_menu_object = nullptr;
    PauseMenu* pause_menu_object = nullptr;
    GameoverMenu* gameover_menu_object = nullptr;

    if (menu_type == "MainMenu")
        main_menu_object = static_cast<MainMenu*>(choice_menu_object);

    else if (menu_type == "PauseMenu")
        pause_menu_object = static_cast<PauseMenu*>(choice_menu_object);

    else if (menu_type == "GameoverMenu")
        GameoverMenu* gameover_menu_object = static_cast<GameoverMenu*>(choice_menu_object);

    else return;

    // get menu choice
    unsigned short int menu_choice = 0;
    if (main_menu_object != nullptr)
        menu_choice = main_menu_object->get_menu_choice();

    else if (pause_menu_object != nullptr)
        menu_choice = pause_menu_object->get_menu_choice();

    else if (pause_menu_object != nullptr)
        menu_choice = gameover_menu_object->get_menu_choice();

    else return;

    // menu choices: 0 - endless, 1 - bossfight, 2 - campaign,
    // 3 - customize, 4 - highscores, 5 - settings
    switch (menu_choice)
    {
        case 0: // endless
            // Handle endless menu choice here
            break;

        case 1: // bossfight
            // Handle bossfight menu choice here
            break;

        case 2: // campaign
            // Handle campaign menu choice here
            break;

        case 3: // customize
            // Handle customize menu choice here
            break;

        case 4: // highscores
            // Handle highscores menu choice here
            break;

        case 5: // settings
            // Handle settings menu choice here
            break;

        default:
            // Handle invalid menu choice here
            break;
    }

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
    //
    GameStructure* game_structure = new GameStructure();

    while (!glfwWindowShouldClose(m_window))
    {
        // ===  process user choices

        // Render here
        game_structure->render_current_menu();

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
    // GameWindow* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
    // Track mouse motion here
    // xpos and ypos represent the new mouse position
}

void GameWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// ===============  methods implementation ======================

// Abstract menu
AbstractMenu::AbstractMenu(unsigned short int max_labels)
{
    this->current_label = 0;
    this->menu_choice = -1;
    this->max_labels = max_labels;
}

AbstractMenu::~AbstractMenu()
{
    // abstract menu destructor
}

//
void AbstractMenu::increment_current_label(short int n)
{
    // implement ring structure of main menu
    this->current_label = (this->current_label + n + this->max_labels) % this->max_labels;
}

// setters/getters
void AbstractMenu::set_current_label(unsigned short int n)
{
    // check if outside of the set of possible values
    this->current_label = n;
    this->current_label %= this->max_labels;    // circular structure of menu
}

void AbstractMenu::set_menu_choice(unsigned short int n)
{
    bool is_legal = (n > this->max_labels) ? false : true;
    if (is_legal)
        this->current_label = n;
}

unsigned short int AbstractMenu::get_current_label() const
{
    return this->current_label;
}

unsigned short int AbstractMenu::get_menu_choice() const
{
    return this->menu_choice;
}

void AbstractMenu:: menu_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    return;
}

void AbstractMenu::static_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    AbstractMenu* abstract_menu = static_cast<AbstractMenu*>(glfwGetWindowUserPointer(window));
    abstract_menu->menu_key_callback(window, key, scancode, action, mods);
}

void AbstractMenu::menu_register_key_callback(GLFWwindow* window)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, static_key_callback);
    std::cout << "menu initialized. max labels:" << this->max_labels << '\n';
}

void AbstractMenu::render_menu() const
{
    return;
}

// ============= Main menu ==============
MainMenu::MainMenu(unsigned short int max_labels) : AbstractMenu(max_labels)
{
    // main menu constructor by default
    this->current_label = 0;    // 0 is a Endless mode
    this->menu_choice = -1;     // -1 will be treated as NULL-like value
    this->max_labels = max_labels;
}

MainMenu::~MainMenu()
{
    // main menu destructor
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
    void* font = GLUT_BITMAP_HELVETICA_18;
    //
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
        color_endless_font, color_endless_bg, color_endless_border,
        font
    );
    // Boss-fight
    gl_draw_menu_label_centered(
        "Bossfight", 0,  label_start_heigh + label_height_step, label_width, label_height,
        color_bossfight_font, color_bossfight_bg, color_bossfight_border,
        font
    );
    // Campaign
    gl_draw_menu_label_centered(
        "Campaign", 0, label_start_heigh + label_height_step * 2, label_width, label_height,
        color_campaign_font, color_campaign_bg, color_campaign_border,
        font
    );
    // Customize
    gl_draw_menu_label_centered(
        "Customize", 0, label_start_heigh + label_height_step * 3, label_width, label_height,
        color_customize_font, color_customize_bg, color_customize_border,
        font
    );
    // Highscores
    gl_draw_menu_label_centered(
        "Highscores", 0, label_start_heigh + label_height_step * 4, label_width, label_height,
        color_highscores_font, color_highscores_bg, color_highscores_border,
        font
    );
    // Settings
    gl_draw_menu_label_centered(
        "Settings", 0, label_start_heigh + label_height_step * 5, label_width, label_height,
        color_settings_font, color_settings_bg, color_settings_border,
        font
    );
}

void MainMenu::render_menu() const
{
    // render all components defined earlier
    this->render_menu_background(); // bg first because it must be the "furthest" layer of the rendering
    this->render_game_logo();
    this->render_menu_labels();
}

// glfw key callback functions
void MainMenu::menu_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //MainMenu* mainMenu = static_cast<MainMenu*>(glfwGetWindowUserPointer(window));
    std::cout << this->max_labels << '\n';

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

// ===== Pause methods definition
PauseMenu::PauseMenu(unsigned short int max_labels) : AbstractMenu(max_labels)
{
    this->max_labels = max_labels;
}

PauseMenu::~PauseMenu()
{

}

//
void PauseMenu::pause_call()
{

}

void PauseMenu::render_pause_labels() const
{

}

void PauseMenu::render_menu() const
{

}

// separate key callback glfw functions
void PauseMenu::menu_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    return;
}

// ==== GameOver menu methods definition

GameoverMenu::GameoverMenu(unsigned short int max_labels) : AbstractMenu(max_labels)
{
    this->max_labels = max_labels;
}

GameoverMenu::~GameoverMenu()
{

}

// GLFW callback functions
void GameoverMenu::menu_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

// render functions
void GameoverMenu::render_menu() const
{

}

///////////////////////////////////////////////////////
// ==== GAME MODES METHODS IMPLEMENTATION ============

EndlessMode::EndlessMode()
{
    // initialize player

    // depending on difficulty initialize enemy pool
    return;
}

EndlessMode::~EndlessMode()
{

}

void EndlessMode::gl_render_score() const
{

}

void EndlessMode::endless_mode_game_loop()
{
    bool game_over = false;

    while (!game_over)
    {
        break;
    }
}

//////////////////////////////////////////////////////
// ==== GAME STRUCTURE METHODS ============

MenuNode::MenuNode(const char* menu_name, void* menu_object)
{
    this->menu_name = menu_name;
    this->menu_object_ptr = menu_object;
    this->prev_node = nullptr;
}

MenuNode::MenuNode()
{
    this->menu_name = "";
    this->menu_object_ptr = nullptr;
    this->prev_node = nullptr;
}

MenuNode::~MenuNode()
{
    // free allocated memory
    if (this->menu_object_ptr == nullptr)
        return;

    // Cast menu object to the appropriate menu type
    if (strcmp(this->menu_name, "MainMenu") == 0)
    {
        MainMenu* main_menu = static_cast<MainMenu*>(this->menu_object_ptr);
        delete main_menu;
    }
    else if (strcmp(this->menu_name, "PauseMenu") == 0)
    {
        PauseMenu* pause_menu = static_cast<PauseMenu*>(this->menu_object_ptr);
        delete pause_menu;
    }
    else if (strcmp(this->menu_name, "GameoverMenu") == 0)
    {
        GameoverMenu* gameover_menu = static_cast<GameoverMenu*>(this->menu_object_ptr);
        delete gameover_menu;
    }
    // add more menu types later
}

// getters/setters
const char* MenuNode::get_menu_name() const
{
    return this->menu_name;
}

void* MenuNode::get_menu_object_ptr() const
{
    return this->menu_object_ptr;
}

MenuNode* MenuNode::get_prev_node() const
{
    return this->prev_node;
}

void MenuNode::set_menu_name(const char* menu_name)
{
    this->menu_name = menu_name;
}

void MenuNode::set_menu_object_ptr(void* menu_object_ptr)
{
    this->menu_object_ptr = menu_object_ptr;
}

void MenuNode::set_prev_node(MenuNode* prev_node)
{
    this->prev_node = prev_node;
}

// ===
GameStructure::GameStructure()
{
    current_menu = nullptr; // Initialize current_menu to nullptr or a valid initial value
    GLFWwindow* current_window = glfwGetCurrentContext();
    // main menu of 6 labels is current by default
    unsigned short int main_menu_labels = 6;
    MainMenu* main_menu_object = new MainMenu(main_menu_labels);
    // make main menu current window in game structure
    MenuNode* main_menu_node = new MenuNode("MainMenu", main_menu_object);
    menu_node_push(main_menu_node); // Push the main menu node onto the stack
    main_menu_object->menu_register_key_callback(current_window);
}

GameStructure::~GameStructure()
{
    return;
}

// basic stack functions
void GameStructure::menu_node_push(MenuNode* menu_node)
{
    if (current_menu == nullptr)
    {
        // If the stack is empty, set the current_menu to the new menu_node
        this->current_menu = menu_node;
        menu_node->set_prev_node(nullptr);
        return;
    }
    // Make the new menu_node the current_menu and update the previous node
    menu_node->set_prev_node(this->current_menu);
    this->current_menu = menu_node;
}

MenuNode* GameStructure::menu_node_pop()
{
    if (current_menu == nullptr)
    {
        // Stack is empty, return nullptr
        return nullptr;
    }
    else
    {
        // Get the current_menu and update current_menu to the previous node
        MenuNode* popped_menu = current_menu;
        current_menu = current_menu->get_prev_node();
        return popped_menu;
    }
}

void GameStructure::menu_node_remove()
{
    if (this->current_menu == nullptr)
    {
        // Stack is empty, nothing to remove
        return;
    }
    MenuNode* temp = this->current_menu; // get pointer to the current stack element
    current_menu = current_menu->get_prev_node();
    // free memory by pointer saved?
    delete temp; // delete menu node
}

// getters/setters
MenuNode* GameStructure::get_current_menu() const
{
    return this->current_menu;
}

void GameStructure::render_current_menu() const
{
    if (this->current_menu == nullptr)
        return;

    // type casting depending on menu object class
    const char* menu_type = this->current_menu->get_menu_name();
    void* menu_object_ptr = this->current_menu->get_menu_object_ptr();
    MainMenu* menu_object_casted_main = nullptr;
    PauseMenu* menu_object_casted_pause = nullptr;
    GameoverMenu* menu_object_casted_gameover = nullptr;

    if (strcmp(menu_type, "MainMenu") == 0)
        menu_object_casted_main = static_cast<MainMenu*>(menu_object_ptr);
    else if (strcmp(menu_type, "PauseMenu") == 0)
        menu_object_casted_pause = static_cast<PauseMenu*>(menu_object_ptr);
    else if (strcmp(menu_type, "GameoverMenu") == 0)
        menu_object_casted_gameover = static_cast<GameoverMenu*>(menu_object_ptr);
    else
        return;

    if (menu_object_casted_main != nullptr)
        menu_object_casted_main->render_menu();
    else if (menu_object_casted_pause != nullptr)
        menu_object_casted_pause->render_menu();
    else if (menu_object_casted_gameover != nullptr)
        menu_object_casted_gameover->render_menu();

}
/////////////////////////////////////////////////////////////////////////////////////////////////////
