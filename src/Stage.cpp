//
// Created by croissant on 25/02/2022.
//

#include "Stage.h"
#include "graphics.h"
#include <iostream>
#include <ctime>

Stage* Stage::mainStage = nullptr;

Stage::Stage(const string& name, int x, int y, int width, int height)
        : name(name), x(x), y(y), width(width), height(height),
          onFocusHandlers(new std::list<context_event::focusHandler>),
          onEnterHandlers(new std::list<context_event::enterHandler>),
          onKeyHandlers(new std::list<context_event::keyHandler>),
          onCharHandlers(new std::list<context_event::charHandler>),
          onClickHandlers(new std::list<context_event::clickHandler>),
          onScrollHandlers(new std::list<context_event::scrollHandler>),
          onMotionHandlers(new std::list<context_event::motionHandler>) {
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
    delete onFocusHandlers;
    delete onEnterHandlers;
    delete onKeyHandlers;
    delete onCharHandlers;
    delete onClickHandlers;
    delete onScrollHandlers;
    delete onMotionHandlers;
}

GLFWwindow* Stage::getWindow() const {
    return window;
}

void Stage::addOnStageFocus(context_event::focusHandler handler) {
    onFocusHandlers->push_front(handler);
}

void Stage::addOnStageEnter(context_event::enterHandler handler) {
    onEnterHandlers->push_front(handler);
}

void Stage::addOnKey(context_event::keyHandler handler) {
    onKeyHandlers->push_front(handler);
}

void Stage::addOnChar(context_event::charHandler handler) {
    onCharHandlers->push_front(handler);
}

void Stage::addOnClick(context_event::clickHandler handler) {
    onClickHandlers->push_front(handler);
}

void Stage::addOnScroll(context_event::scrollHandler handler) {
    onScrollHandlers->push_front(handler);
}

void Stage::addOnMotion(context_event::motionHandler handler) {
    onMotionHandlers->push_front(handler);
}

void Stage::clearOnFocus() {
    onFocusHandlers->clear();
}

void Stage::clearOnEnter() {
    onEnterHandlers->clear();
}

void Stage::clearOnKey() {
    onKeyHandlers->clear();
}

void Stage::clearOnChar() {
    onCharHandlers->clear();
}

void Stage::clearOnClick() {
    onClickHandlers->clear();
}

void Stage::clearOnScroll() {
    onScrollHandlers->clear();
}

void Stage::clearOnMotion() {
    onMotionHandlers->clear();
}

void Stage::onFocus(GLFWwindow* window, int focused) {
    auto handlers = mainStage->onFocusHandlers;
    for (auto handler: *handlers) {
        handler(context_event::focusEvent{window, focused});
    }
}

void Stage::onEnter(GLFWwindow* window, int entered) {
    auto handlers = mainStage->onEnterHandlers;
    for (auto handler: *handlers) {
        handler(context_event::enterEvent{window, entered});
    }
}

void Stage::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto handlers = mainStage->onKeyHandlers;
    for (auto handler: *handlers) {
        handler(context_event::keyEvent{window, key, scancode, action, mods});
    }
}

void Stage::onChar(GLFWwindow* window, unsigned int codepoint) {
    auto handlers = mainStage->onCharHandlers;
    for (auto handler: *handlers) {
        handler(context_event::charEvent{window, codepoint});
    }
}

void Stage::onClick(GLFWwindow* window, int button, int action, int mods) {
    auto handlers = mainStage->onClickHandlers;
    for (auto handler: *handlers) {
        handler(context_event::clickEvent{window, button, action, mods});
    }
}

void Stage::onScroll(GLFWwindow* window, double xoffset, double yoffset) {
    auto handlers = mainStage->onScrollHandlers;
    for (auto handler: *handlers) {
        handler(context_event::scrollEvent{window, xoffset, yoffset});
    }
}

void Stage::onMotion(GLFWwindow* window, double xpos, double ypos) {
    auto handlers = mainStage->onMotionHandlers;
    for (auto handler: *handlers) {
        handler(context_event::motionEvent{window, xpos, ypos});
    }
}


