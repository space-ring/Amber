//
// Created by croissant on 25/02/2022.
//

#include "Stage.h"
#include "graphics.h"

Stage::Stage(const char *name, int x, int y, int width, int height)
        : name(name), x(x), y(y), width(width), height(height) {

}

void Stage::run() {

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    terminate();
}

void Stage::terminate() {
    glfwTerminate();
    delete window;
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
