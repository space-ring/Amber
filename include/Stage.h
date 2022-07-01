//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_STAGE_H
#define ENGINE_STAGE_H

#include "graphics.h"
#include <string>
#include "event.h"
#include <list>
#include <queue>
#include <map>
#include "Engine.h"

class Stage {
    using string = std::string;
    template<class T>
    using vector = std::vector<T>;

private:
    Engine* root;
    string name;
    int x, y, width, height;
    GLFWwindow* window = nullptr;

    void render();

    void poll();

    void update();

public:

    bool focused;

    Stage(Engine* root, const string& name, int x, int y, int width, int height);

    virtual ~Stage();

    /* Initialises GLFW window and OpenGL context */
    void init();

    /* Enters the main render loop */
    void run();

    void terminate();

    void show();

    void hide();

    GLFWwindow* getWindow() const;

};


#endif //ENGINE_STAGE_H
