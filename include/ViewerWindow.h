#pragma once

#include <functional>
#include <cstdint>
#include <string>
#include <GLFW/glfw3.h>

// модифікатори клавіш
enum class Modifier : uint8_t {
    None    = 0,
    Shift   = 1 << 0,
    Control = 1 << 1,
    Alt     = 1 << 2,
    Super   = 1 << 3
};

inline Modifier operator|(Modifier a, Modifier b)
{
    return static_cast<Modifier>(
        static_cast<uint8_t>(a) | static_cast<uint8_t>(b)
    );
}

inline Modifier& operator|=(Modifier& a, Modifier b)
{
    a = a | b;
    return a;
}

// дія клавіші/кнопки
enum class Action : uint8_t {
    Press,
    Release,
    Repeat
};

// кнопки миші
enum class ButtonCode : uint8_t {
    Button_0 = 0,
    Button_1,
    Button_2,
    Button_3,
    Button_4,
    Button_5,
    Button_6,
    Button_7,
    Unknown
};

// основні коди клавіш
enum class KeyCode : int32_t {
    Unknown = -1,
    Space   = GLFW_KEY_SPACE,
    Escape  = GLFW_KEY_ESCAPE,
    Left    = GLFW_KEY_LEFT,
    Right   = GLFW_KEY_RIGHT,
    Up      = GLFW_KEY_UP,
    Down    = GLFW_KEY_DOWN
};

inline Modifier modifiersFromGlfw(int mods)
{
    Modifier result = Modifier::None;
    if (mods & GLFW_MOD_SHIFT)   result |= Modifier::Shift;
    if (mods & GLFW_MOD_CONTROL) result |= Modifier::Control;
    if (mods & GLFW_MOD_ALT)     result |= Modifier::Alt;
    if (mods & GLFW_MOD_SUPER)   result |= Modifier::Super;
    return result;
}

class ViewerWindow {
public:
    using KeyCallback         = std::function<void(KeyCode, Action, Modifier)>;
    using MouseButtonCallback = std::function<void(ButtonCode, Action, Modifier)>;
    using CursorPosCallback   = std::function<void(double, double)>;
    using ScrollCallback      = std::function<void(double, double)>;

    ViewerWindow(const std::string& title, int width, int height);
    ~ViewerWindow();

    ViewerWindow(const ViewerWindow&) = delete;
    ViewerWindow& operator=(const ViewerWindow&) = delete;

    int getWidth()  const { return m_width; }
    int getHeight() const { return m_height; }

    GLFWwindow* getGLFWHandle() const { return m_ptrHandle; }

    void setKeyCallback        (KeyCallback cb)         { m_keyCallback        = std::move(cb); }
    void setMouseButtonCallback(MouseButtonCallback cb) { m_mouseButtonCallback = std::move(cb); }
    void setCursorPosCallback  (CursorPosCallback cb)   { m_cursorPosCallback  = std::move(cb); }
    void setScrollCallback     (ScrollCallback cb)      { m_scrollCallback     = std::move(cb); }

    bool shouldClose() const;
    void pollEvents() const;
    void waitEvents() const;
    void swapBuffers() const;

private:
    GLFWwindow* m_ptrHandle{};
    int m_width{};
    int m_height{};

    KeyCallback         m_keyCallback;
    MouseButtonCallback m_mouseButtonCallback;
    CursorPosCallback   m_cursorPosCallback;
    ScrollCallback      m_scrollCallback;

    friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    friend void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    friend void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
