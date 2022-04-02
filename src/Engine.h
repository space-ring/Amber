//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <map>
#include "Stage.h"
#include "Scene.h"
#include "graphics.h"
#include "Shader.h"

class Engine {

private:
    Stage *stage;
    std::map<unsigned int, Scene *> *scenes;
    std::map<unsigned int, Shader *> *shaders;
    bool running;

    void init();


public:
    Engine(const char *name, int x, int y, int width, int height);

    virtual ~Engine();

    void run();

    void kill();

    Stage *const getStage() const;


};


#endif //ENGINE_ENGINE_H
