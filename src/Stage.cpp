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
    glfwSetCursorEnterCallback(window, EventManager::onGLFWevent<window_events::EnterEvent, int>);
    glfwSetWindowCloseCallback(window, EventManager::onGLFWevent<window_events::CloseEvent>);
    glfwSetWindowFocusCallback(window, EventManager::onGLFWevent<window_events::FocusEvent, int>);
    glfwSetKeyCallback(window, EventManager::onGLFWevent<window_events::KeyEvent, int, int, int, int>);
    glfwSetCharCallback(window, EventManager::onGLFWevent<window_events::CharEvent, unsigned int>);
    glfwSetMouseButtonCallback(window, EventManager::onGLFWevent<window_events::ClickEvent, int, int, int>);
    glfwSetScrollCallback(window, EventManager::onGLFWevent<window_events::ScrollEvent, double, double>);
    glfwSetCursorPosCallback(window, EventManager::onGLFWevent<window_events::MotionEvent, double, double>);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void Stage::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (front) front->render();
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
    for (auto& scene: *scenes) {
        delete scene.second;
    }
    delete scenes;
    glfwTerminate();
}

GLFWwindow* Stage::getWindow() const {
    return window;
}

void Stage::addScene(const string& id, Scene* scene) {
    scenes->insert(std::pair(id, scene));
}

void Stage::setFrontScene(const string& scene) {
    if (scenes->contains(scene)) front = scenes->at(scene);
}

