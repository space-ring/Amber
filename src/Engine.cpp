//
// Created by croissant on 25/02/2022.
//

#include <iostream>
#include "Engine.h"
#include "Shader.h"


void Engine::operator()() { //launching the thread
    run();
}

void Engine::run() {
    init();
    stage->run();
}

void Engine::init() {
    // set up GL context todo move this to constructor?
    stage->init();

    //set up defaults
    //SHADER
//    shaders->insert(std::pair<int, Shader *>(0, Shader::getDefault()));

    //MESH
    //TEXTURE
    stage->show();
}

Engine::Engine(const char *name, int x, int y, int width, int height)
        : scenes(new std::map<unsigned int, Scene *>),
          shaders(new std::map<unsigned int, Shader *>),
          running(false) {
    stage = new Stage(name, x, y, width, height);
}

Engine::~Engine() {
    kill();
    delete scenes;
    delete shaders;
}

void Engine::kill() {
    //todo unload resources
    glfwWindowShouldClose(stage->getWindow());
}

Stage *Engine::getStage() const {
    return stage;
}