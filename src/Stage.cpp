//
// Created by croissant on 25/02/2022.
//

#include "Stage.h"
#include "graphics.h"
#include <iostream>
#include <ctime>

Stage::Stage(const char *name, int x, int y, int width, int height)
        : name(name), x(x), y(y), width(width), height(height) {
}

void Stage::run() {
    std::cout << "stage is running" << std::endl;

    std::time_t start = std::time(nullptr);
    int frames = 0;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
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
    std::cout << "Killing\n";
//    glfwDestroyWindow(window);
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
    glfwSwapInterval(1);
    glClearColor(1, 0, 0, 1);
    glViewport(0, 0, width, height);

    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

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
