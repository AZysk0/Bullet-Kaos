#include "GLFWWindow.h"
//#include "glfw_window.h"


GLFWWindow::GLFWWindow(int width, int height, const char* title)
    : m_width(width), m_height(height), m_title(title), m_window(nullptr) {}


bool GLFWWindow::Initialize()
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

void GLFWWindow::Run()
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

void GLFWWindow::Shutdown()
{
    glfwTerminate();
}

void GLFWWindow::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLFWWindow* glfwWindow = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void GLFWWindow::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


