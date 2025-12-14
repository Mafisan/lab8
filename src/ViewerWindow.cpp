#include "ViewerWindow.h"

#include <stdexcept>

static ViewerWindow* getViewer(GLFWwindow* window)
{
    return static_cast<ViewerWindow*>(glfwGetWindowUserPointer(window));
}

// --- колбеки GLFW ---

void keyCallback(GLFWwindow* window, int key, int, int action, int mods)
{
    ViewerWindow* self = getViewer(window);
    if (!self || !self->m_keyCallback) return;

    Action act;
    switch (action) {
    case GLFW_PRESS:  act = Action::Press;  break;
    case GLFW_REPEAT: act = Action::Repeat; break;
    case GLFW_RELEASE:
    default:          act = Action::Release; break;
    }

    KeyCode  keyCode  = static_cast<KeyCode>(key);
    Modifier modifier = modifiersFromGlfw(mods);

    self->m_keyCallback(keyCode, act, modifier);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    ViewerWindow* self = getViewer(window);
    if (!self || !self->m_mouseButtonCallback) return;

    Action act;
    switch (action) {
    case GLFW_PRESS:  act = Action::Press;  break;
    case GLFW_REPEAT: act = Action::Repeat; break;
    case GLFW_RELEASE:
    default:          act = Action::Release; break;
    }

    ButtonCode btn = ButtonCode::Unknown;
    if (button >= 0 && button <= 7)
        btn = static_cast<ButtonCode>(button);

    Modifier modifier = modifiersFromGlfw(mods);
    self->m_mouseButtonCallback(btn, act, modifier);
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    ViewerWindow* self = getViewer(window);
    if (!self || !self->m_cursorPosCallback) return;
    self->m_cursorPosCallback(xpos, ypos);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    ViewerWindow* self = getViewer(window);
    if (!self || !self->m_scrollCallback) return;
    self->m_scrollCallback(xoffset, yoffset);
}

// --- ViewerWindow ---

ViewerWindow::ViewerWindow(const std::string& title, int width, int height)
    : m_width(width)
    , m_height(height)
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    m_ptrHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_ptrHandle) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_ptrHandle);
    glfwSetWindowUserPointer(m_ptrHandle, this);

    glfwSetKeyCallback       (m_ptrHandle, keyCallback);
    glfwSetMouseButtonCallback(m_ptrHandle, mouseButtonCallback);
    glfwSetCursorPosCallback (m_ptrHandle, cursorCallback);
    glfwSetScrollCallback    (m_ptrHandle, scrollCallback);

    glfwSwapInterval(1);
}

ViewerWindow::~ViewerWindow()
{
    if (m_ptrHandle) {
        glfwDestroyWindow(m_ptrHandle);
        m_ptrHandle = nullptr;
    }
    glfwTerminate();
}

bool ViewerWindow::shouldClose() const
{
    return glfwWindowShouldClose(m_ptrHandle) != 0;
}

void ViewerWindow::pollEvents() const
{
    glfwPollEvents();
}

void ViewerWindow::waitEvents() const
{
    glfwWaitEvents();
}

void ViewerWindow::swapBuffers() const
{
    glfwSwapBuffers(m_ptrHandle);
}
