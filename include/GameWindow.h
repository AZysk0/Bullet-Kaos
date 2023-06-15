#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H
#define TIME_PER_FRAME 1/60

#include <iostream>
#include <GLFW/glfw3.h>
#include <cmath>

#include "GameInstances.h"
#include "animations.h"


class GameWindow {
public:
    GameWindow(int width, int height, const char* title)
        : m_width(width), m_height(height), m_title(title), m_window(nullptr) {}

    ~GameWindow();
    // fundamental graphics functions (methods)
    bool Initialize();
    void Run();
    void Shutdown();

    // game loop helpers
    void game_main_loop();
    void game_setup();

    // draw scene functions
    void gl_draw_main_menu();
    void gl_draw_game_over_menu();

    // GLFW event functions
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

private:
    int m_width;
    int m_height;
    const char* m_title;
    GLFWwindow* m_window;
    //Player* m_player;
};

//
class MainMenu
{
private:
    // flags
public:
    // tracking mouse events
};
//


//////////////////////////////////////////////
// different game loop for each mode
class EndlessMode
{
private:
public:
};

class BossfightMode
{
private:
public:
};

class CampaignMode
{
private:
public:
};

// ======= Other functions ==============
void time_sleep(float t);
void gl_draw_text_window_pos(const char* text, int x, int y, glm::vec3 color);
void gl_draw_text_raster_pos(const char* text, float x, float y, glm::vec3 color);

#endif // GAME_WINDOW_H
