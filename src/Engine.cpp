//
// Created by croissant on 25/02/2022.
//

#include <iostream>
#include "Engine.h"
#include "Shader.h"
#include "Mesh.h"
#include "engineIO.h"
#include "Stage.h"
#include "managers.h"

void Engine::init() {
    stage->init();
    stage->show();
//    assets->buildAll();
    events->addHandler(
            window_event::CloseHandler([&](const window_event::CloseEvent&) { running = false; })
    );
    events->addHandler(
            window_event::FocusHandler([&](const window_event::FocusEvent& e) { stage->focused = e.focused; })
    );
}

void Engine::run() {
    init();

    std::time_t start = std::time(nullptr);
    int frames = 0;

    running = true;
    while (running) {
        stage->poll();
        stage->render();

        ++frames;
        std::time_t now = std::time(nullptr);
        if (std::difftime(now, start) > 1 / 2) { //todo what is going on here
            std::cout << frames << std::endl;
            frames = 0;
            start = std::time(nullptr);
        }
    }
}

Engine::Engine(const string& name, int x, int y, int width, int height) :
        assets(new AssetManager()),
        events(new EventManager()),
        stage(new Stage(this, name, x, y, width, height)) {
}

Engine::~Engine() {
    delete assets;
    delete events;
    delete stage;
}

void Engine::kill() {
    running = false;
}

Stage* Engine::getStage() const {
    return stage;
}