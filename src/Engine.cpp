//
// Created by croissant on 25/02/2022.
//

#include <iostream>
#include "Engine.h"
#include "Shader.h"
#include "Mesh.h"
#include "engineIO.h"

void Engine::run() {
    init();
    stage->run();
    kill();
}

void Engine::init() {
    stage->init();

//    assets->buildAll();

    //set up defaults
    //SHADER
//    Shader::getDefault()->build();
    //MESH
//    Mesh::getDefault()->build();
    //TEXTURE

    stage->show();

    running = true;
}

Engine::Engine(const string& name, int x, int y, int width, int height) :
        stage(new Stage(name, x, y, width, height)) {
}

Engine::~Engine() {
    delete scenes;
    kill();
}

void Engine::kill() {
    if (!running) return;
    running = false;
    delete assets;
    stage->terminate();
    delete stage;
}

Stage* Engine::getStage() const {
    return stage;
}

Scene* Engine::getFrontScene() {
    return frontScene;
}

void Engine::addScene(const string& id, Scene* scene) {
    scenes->insert(std::pair<string, Scene*>(id, scene));
}

Scene* Engine::getFront() {
    return frontScene;
}

const std::mutex& Engine::getQmutex() const {
    return Qmutex;
}
