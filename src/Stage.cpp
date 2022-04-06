//
// Created by croissant on 25/02/2022.
//

#include "Stage.h"
#include "graphics.h"
#include <iostream>
#include <ctime>

Stage *Stage::mainStage = nullptr;

Stage::Stage(const char *name, int x, int y, int width, int height)
        : name(name), x(x), y(y), width(width), height(height) {
    Stage::mainStage = this;
}

void Stage::run() {
    std::time_t start = std::time(nullptr);
    int frames = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render();
        glfwSwapBuffers(window);

        ++frames;
        std::time_t now = std::time(nullptr);
        if (std::difftime(now, start) > 1 / 2) { //todo what is going on here
            std::cout << frames << std::endl;
            frames = 0;
            start = std::time(nullptr);
        }
    }

    terminate();
}

void Stage::terminate() {
//    glfwDestroyWindow(window); //not needed
    glfwTerminate();
}


void Stage::init() {
    // set up GL context
    if (!glfwInit()) {
        exit(1);
    }

    glfwWindowHint(GLFW_VISIBLE, 0);

    window = glfwCreateWindow(width, height, name, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(2);
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        exit(3);
    }

    glfwSetWindowPos(window, x, y);

    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

    glfwSetWindowFocusCallback(window, onFocus);
}

void Stage::render() {

}

void Stage::show() {
    glfwShowWindow(window);
}

void Stage::hide() {
    glfwHideWindow(window);
}

Stage::~Stage() {
    delete name;
}

GLFWwindow *Stage::getWindow() const {
    return window;
}

void Stage::addOnFocus(context_event::focusHandler handler) {
    onFocusHandler = handler;
}

void Stage::clearOnFocus() {

}

void Stage::onFocus(GLFWwindow *window, int focused) {
    mainStage->onFocusHandler(context_event::focusEvent{window, focused});
}

void Stage::onEnter(GLFWwindow *window, int entered) {
    mainStage->onEnterHandler(context_event::enterEvent{window, entered})
}

void Stage::onKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    mainStage->onKeyHandler(context_event::keyEvent{window, key, scancode, action, mods})
}

void Stage::onChar(GLFWwindow *window, unsigned int codepoint) {
    mainStage->onCharHandler(context_event::charEvent{window, codepoint})
}

void Stage::onClick(GLFWwindow *window, int button, int action, int mods) {
    mainStage->onClickHandler(context_event::clickEvent{window, button, action, mods})
}

void Stage::onScroll(GLFWwindow *window, double xoffset, double yoffset) {
    mainstage->onScrollHanlder(context_event::scrollEvent{window, xoffset, yoffset})
}

void Stage::onMotion(GLFWwindow *window, double xpos, double ypos) {
    mainstage->onMotionHanlder(context_event::motionEvent{window, xpos, ypos})
}


