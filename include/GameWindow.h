#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H
#define TIME_PER_FRAME 1/60

#include <iostream>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <cmath>

#include "GameInstances.h"
#include "animations.h"

// ======= Helper functions ==============
// shapes rendering
void gl_draw_bordered_rectangle(float x_pos, float y_pos,
                                float width, float height,
                                glm::vec4 fill_color, glm::vec4 border_color);
// text rendering
void gl_draw_text_window_pos(const char* text, int x, int y, glm::vec3 color);
void gl_draw_text_raster_pos(const char* text, float x, float y, glm::vec3 color);
void gl_draw_menu_label_centered(
    const char* text, float x_pos, float y_pos, float width, float height,
    glm::vec4 font_color, glm::vec4 bg_color, glm::vec4 border_color
);

// game instances forward initialization ========
class Player;
class Enemy;
class EnemyPool;

// game window modes classes
class MainMenu;
class PauseMenu;
class EndlessMode;
class BossfightMode;
class CampaignMode;
//

class GameWindow
{
private:
    int m_width;
    int m_height;
    const char* m_title;
    GLFWwindow* m_window;
    // window flags (call different menus depending on boolean values)
    bool is_current_endless;    // start endless mode gameloop
    bool is_current_bossfight;  // open boss-fight menu
    bool is_current_campaign;   // open level menu
    bool is_current_customize;  // open player customization menu
    bool is_current_settings;   // open settings menu
    bool is_current_main_menu;   // open settings menu
public:
//    GameWindow(int width, int height, const char* title)
//        : m_width(width), m_height(height), m_title(title), m_window(nullptr),
//         {}
    GameWindow(int width, int height, const char* title);
    ~GameWindow();
    // fundamental graphics functions (methods)
    bool Initialize();
    void Run();
    void Shutdown();

    // game loop helpers
    void game_main_loop();
    void game_setup();

    // GLFW event functions
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};

// ============= Designing menus of the game ==============
// Main Menu structure (Tree-like)
// in each game mode you can also call PauseMenu
// MainMenu:
// -- Endless
// -- Bossfight
// ---- BiriBiri
// ---- PussyPunisher
// ---- ETC
// -- Campaign
// ---- Choose Level (1, 2, ..., n)
// -- Customize (Player)
// -- Highscores
// -- Settings (Graphics, Difficulty)
// ======================================================

class MainMenu
{
private:
    unsigned short int current_label;       // label current choice on
    unsigned short int menu_choice;         // being assigned after hitting enter button
    const unsigned short int max_labels=6;  // max labels in menu now
public:
    //
    MainMenu();
    ~MainMenu();
    //
    void increment_current_label(short int n);
    // main menu rendering functions
    void render_menu_background() const;
    void render_game_logo() const;
    void render_menu_labels() const;
    void render_main_menu() const;
    // setters/getters
    void set_current_label(unsigned short int n);
    void set_menu_choice(unsigned short int n);
    unsigned short int get_current_label() const;
    unsigned short int get_menu_choice() const;
    // glfw key callback functions
    void menu_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void static_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods); // static wrapper
    void menu_register_key_callback(GLFWwindow* window); // register key
};


class PauseMenu
{
private:
    unsigned short int current_label;  // label current choice on
    unsigned short int menu_choice;    // being assigned after hitting enter button
    bool is_paused;
public:
    PauseMenu();
    ~PauseMenu();
    //
    void pause_call();  // while loop
    void render_pause_labels() const;
    void render_pause_menu() const;
    // separate key_callback glfw functions
    // static void key_callback(...);
};


//////////////////////////////////////////////
// different game loop for each mode
class EndlessMode
{
private:
    unsigned short int difficulty;  // difficulty responds for initial enemy pool state
    unsigned int current_score;     // track current score
    // add enemy pool, player
public:
    void gl_render_score() const;
    void endless_mode_game_loop();
};


class BossfightMode
{
private:
    unsigned short int difficulty;
public:
    void bossfight_mode_game_loop();
    // different bosses to load
};


class CampaignMode
{
private:
    EnemyPool* enemies;
public:
    CampaignMode();
    ~CampaignMode();
    // level functions
    void load_initial_enemy_pool();
    void load_level_from_file();
    // choose level section
    void render_level_choose_section();
};

// ========================
#endif // GAME_WINDOW_H
