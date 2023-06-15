#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include <iostream>
#include <GLFW/glfw3.h>

class GLFWWindow {
public:
    GLFWWindow(int width, int height, const char* title);

    bool Initialize();
    void Run();
    void Shutdown();

private:
    int m_width;
    int m_height;
    const char* m_title;
    GLFWwindow* m_window;

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};

#endif

/*
class GLFWWindow
{
    public:
        GLFWWindow();
        virtual ~GLFWWindow();

    protected:

    private:
};
*/

