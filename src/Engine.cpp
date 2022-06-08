//
// Created by croissant on 25/02/2022.
//

#include <iostream>
#include "Engine.h"
#include "Shader.h"
#include "Mesh.h"
#include "engineIO.h"


//void Engine::operator()(const string& manifest) {
//    run(manifest);
//}

void Engine::run() {
    init();
    stage->run();
}

void Engine::init() {
    // set up GL context todo move this to constructor?
    stage->init();

    //set up defaults
    //SHADER
    assets->addShader("", *Shader::getDefault()->build());
//    MESH
    assets->addMesh("", *Mesh::getDefault()->build());
    //todo TEXTURE

    Shader* wire = assets->getShader("wire");
    wire->build();

    stage->show();

    running = true;
}

Engine::Engine(const string& name, int x, int y, int width, int height) {
    stage = new Stage(name, x, y, width, height);
}

Engine::~Engine() {
    kill();
    delete scenes;
    delete assets;
}

void Engine::kill() {
    //todo unload resources
    glfwWindowShouldClose(stage->getWindow());
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
