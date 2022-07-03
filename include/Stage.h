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

private:
    Engine* root;
    string name;
    int x, y, width, height;
    GLFWwindow* window = nullptr;
    std::map<string, Scene*>* scenes = new std::map<string, Scene*>;
    Scene* front = nullptr;

public:

    bool focused;

    Stage(Engine* root, const string& name, int x, int y, int width, int height);

    virtual ~Stage();

    /* Initialises GLFW window and OpenGL context */
    void init();

    void render();

    void poll();

    void update();

    void show();

    void hide();

    GLFWwindow* getWindow() const;

    void addScene(const string& id, Scene* scene);

    void setFrontScene(const string& scene);

};


#endif //ENGINE_STAGE_H
