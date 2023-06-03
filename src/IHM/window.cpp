#include "IHM/window.h"

#include <string>
#include <stdexcept>
#include <iostream>

static bool is_init = false;

void Window::init() {
    if(!glfwInit()) {
        throw std::runtime_error("Could not initialize glfw");
    }
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "Error (" << error << "): " << description << std::endl;
    });
    is_init = true;
}

Window::Window(unsigned int width, unsigned int height, const char *title) {
    if(!is_init) Window::init();

    _width = width;
    _height = height;
    _title = title;
    _aspect_ratio = float(width) / float(height);

    _glwin = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if(!_glwin) {
        throw std::runtime_error("Could not create a window");
    }
    glfwSetWindowUserPointer(_glwin, this);
    _setEventCallbacks();
    makeContextCurrent();
}
Window::~Window() {
    glfwDestroyWindow(_glwin);
}

void Window::_setEventCallbacks() {
    glfwSetKeyCallback(_glwin, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Window* this_window = (Window*)glfwGetWindowUserPointer(window);
        this_window->on_key(key, scancode, action, mods);
    });
    glfwSetMouseButtonCallback(_glwin, [](GLFWwindow* window, int button, int action, int mods) {
        Window* this_window = (Window*)glfwGetWindowUserPointer(window);
        this_window->on_mouse_button(button, action, mods);
    });
    glfwSetCursorPosCallback(_glwin, [](GLFWwindow* window, double xpos, double ypos) {
        Window* this_window = (Window*)glfwGetWindowUserPointer(window);
        this_window->on_mouse_move((xpos/this_window->width * (-2) + 1) * this_window->aspect_ratio, ypos/this_window->height * (-2) + 1);
    });
    glfwSetScrollCallback(_glwin, [](GLFWwindow* window, double xoffset, double yoffset) {
        Window* this_window = (Window*)glfwGetWindowUserPointer(window);
        this_window->on_scroll(xoffset, yoffset);
    });
    glfwSetWindowSizeCallback(_glwin, [](GLFWwindow* window, int width, int height) {
        Window* this_window = (Window*)glfwGetWindowUserPointer(window);
        this_window->updateSize(width, height);
        this_window->on_window_resize(width, height);
    });
}

int Window::getKey(int key) {
    return glfwGetKey(_glwin, key);
}
void Window::makeContextCurrent() {
    glfwMakeContextCurrent(_glwin);
}
bool Window::shouldClose() {
    return glfwWindowShouldClose(_glwin);
}
void Window::updateSize(unsigned int width, unsigned int height) {
    _width = width > 0 ? width : _width;
    _height = height > 0 ? height : _height;
    _aspect_ratio = float(_width) / float(_height);
}

void Window::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::refresh() {
    glfwSwapBuffers(_glwin);
}

void Window::pollEvents() {
    glfwPollEvents();
}

