//
// Created by croissant on 25/02/2022.
//

#include "Stage.h"
#include "graphics.h"
#include "EventManager.h"
#include <iostream>
#include <ctime>

namespace Amber {
    Stage::Stage(Engine* root, const string& name, int x, int y, int width, int height)
            : root(root), name(name), x(x), y(y), width(width), height(height) {
    }

    Stage::~Stage() {
        //todo removed delete scene here. ownership.
        delete scenes;
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
//        glfwSwapInterval(1);
        printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

        glfwSetWindowUserPointer(window, root);

        //game_events
        glfwSetCursorEnterCallback(window, onGLFWevent<window_events::EnterEvent, int>);
        glfwSetWindowCloseCallback(window, onGLFWevent<window_events::CloseEvent>);
        glfwSetWindowFocusCallback(window, onGLFWevent<window_events::FocusEvent, int>);
        glfwSetKeyCallback(window, onGLFWevent<window_events::KeyEvent, int, int, int, int>);
        glfwSetCharCallback(window, onGLFWevent<window_events::CharEvent, unsigned int>);
        glfwSetMouseButtonCallback(window, onGLFWevent<window_events::ClickEvent, int, int, int>);
        glfwSetScrollCallback(window, onGLFWevent<window_events::ScrollEvent, double, double>);
        glfwSetCursorPosCallback(window, onGLFWevent<window_events::MotionEvent, double, double>);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }

    void Stage::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (front) {
            front->build(); //todo poor
            front->render();
        }
        glfwSwapBuffers(window);
    }

    void Stage::update() {
        if (front) front->update();
    }

    void Stage::poll() {
        if (focused) glfwPollEvents();
        else glfwWaitEvents();
    }

    void Stage::pick() {
        int wx, wy, ww, wh;
        double dx, dy;
        glfwGetWindowPos(window, &wx, &wy);
        glfwGetWindowSize(window, &ww, &wh);
        glfwGetCursorPos(window, &dx, &dy);
        if (wx <= (int) dx && (int) dx < (wx + ww) && wy <= (int) dy && (int) dy < (wy + wh))
            if (front) front->pick(dx, dy);
    }

    void Stage::show() {
        glfwShowWindow(window);
    }

    void Stage::hide() {
        glfwHideWindow(window);
    }

    GLFWwindow* Stage::getWindow() const {
        return window;
    }

    void Stage::addScene(const string& id, Scene* scene) {
        scenes->insert(std::pair(id, scene));
    }

    void Stage::setFrontScene(const string& scene) {
        if (scenes->contains(scene)) {
            if (front) front->hide();
            front = scenes->at(scene);
            front->show();
        }
    }

    Scene* Stage::getFront() const {
        return front;
    }

}