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
    std::map<unsigned int, Scene *> *scenes;
    std::map<unsigned int, Shader *> *shaders;
    bool running;

    void init();

public:

    Stage *stage;
    Engine(const char *name, int x, int y, int width, int height);

    void operator()();

    virtual ~Engine();

    /* Initialises all listed resources, scenes and stages. Enters main render loop */
    void run();

    /* Requests for the window to shut down */
    void kill();

    Stage *getStage() const;
};


#endif //ENGINE_ENGINE_H
