#pragma once

#include "IHM/base.h"

#include <functional>

class Window {
private:
    GLFWwindow* _glwin;
    unsigned int _width, _height;
    const char* _title;
    float _aspect_ratio;

    void _setEventCallbacks();
public:
    const unsigned int& width = _width;
    const unsigned int& height = _height;
    const float& aspect_ratio = _aspect_ratio;
    GLFWwindow*& glwin = _glwin;

    std::function<void (int key, int scancode, int action, int mods)> on_key = [](int, int, int, int) {};
    std::function<void (int button, int action, int mods)> on_mouse_button = [](int, int, int) {};
    std::function<void (double xpos, double ypos)> on_mouse_move = [](double, double) {};
    std::function<void (double xoffset, double yoffset)> on_scroll = [](double, double) {};
    std::function<void (int width, int height)> on_window_resize = [](int width, int height) {};

    Window(unsigned int width = 800, unsigned int height = 600, const char *title = "A window");
    ~Window();

    void makeContextCurrent();
    bool shouldClose();
    void updateSize(unsigned int width = 0, unsigned int height = 0);

    void clear();
    void refresh();

static void pollEvents();
static void init();
};
