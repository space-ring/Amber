//
// Created by croissant on 25/02/2022.
//

#include "Stage.h"
#include "graphics.h"
#include "EventManager.h"
#include <iostream>
#include <ctime>

Stage::Stage(Engine* root, const string& name, int x, int y, int width, int height)
        : root(root), name(name), x(x), y(y), width(width), height(height) {
}

void Stage::run() {
    std::time_t start = std::time(nullptr);
    int frames = 0;

    while (!glfwWindowShouldClose(window)) {
        poll();
        render();

        ++frames;
        std::time_t now = std::time(nullptr);
        if (std::difftime(now, start) > 1 / 2) { //todo what is going on here
            std::cout << frames << std::endl;
            frames = 0;
            start = std::time(nullptr);
        }
    }
}

void Stage::terminate() {
    glfwTerminate();
}

void Stage::init() {
    // set up GL context
    if (!glfwInit()) {
        exit(1);
    }

    glfwWindowHint(GLFW_VISIBLE, 0);

    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(2);
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        exit(3);
    }

    glfwSetWindowPos(window, x, y);
    glfwSwapInterval(1);
    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

    glfwSetWindowUserPointer(window, root);

    //events
    glfwSetCursorEnterCallback(window, EventManager::onGLFWevent<window_event::EnterEvent, int>);
    glfwSetWindowCloseCallback(window, EventManager::onGLFWevent<window_event::CloseEvent>);
    glfwSetWindowFocusCallback(window, EventManager::onGLFWevent<window_event::FocusEvent, int>);
    glfwSetKeyCallback(window, EventManager::onGLFWevent<window_event::KeyEvent, int, int, int, int>);
    glfwSetCharCallback(window, EventManager::onGLFWevent<window_event::CharEvent, unsigned int>);
    glfwSetMouseButtonCallback(window, EventManager::onGLFWevent<window_event::ClickEvent, int, int, int>);
    glfwSetScrollCallback(window, EventManager::onGLFWevent<window_event::ScrollEvent, double, double>);
    glfwSetCursorPosCallback(window, EventManager::onGLFWevent<window_event::MotionEvent, double, double>);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Stage::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
}

void Stage::poll() {
    if (focused) {
        glfwPollEvents();
    } else glfwWaitEvents();
}

void Stage::update() {

}

void Stage::show() {
    glfwShowWindow(window);
}

void Stage::hide() {
    glfwHideWindow(window);
}

Stage::~Stage() {
}

GLFWwindow* Stage::getWindow() const {
    return window;
}

