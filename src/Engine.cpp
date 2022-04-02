//
// Created by croissant on 25/02/2022.
//

#include <iostream>
#include "Engine.h"
#include "Shader.h"


void Engine::init() {
    // set up GL context
    stage->init();

    //set up defaults
    //SHADER
    shaders->insert(std::pair<int, Shader *>(0, Shader::getDefault()));

    //MESH
    //TEXTURE
    stage->show();
}

Engine::Engine(const char *name, int x, int y, int width, int height) :
        scenes(new std::map<unsigned int, Scene *>),
        shaders(new std::map<unsigned int, Shader *>),
        running(false) {

    stage = new Stage(name, x, y, width, height);
}

Engine::~Engine() {
    delete stage;
}

void Engine::run() {
    init();

    stage->run();
}

void Engine::kill() {
    //todo unload resources
    stage->terminate();
}

Stage *const Engine::getStage() const {
    return stage;
}
