//
// Created by croissant on 25/02/2022.
//

#include <iostream>
#include "Engine.h"
#include "Shader.h"
#include "Mesh.h"
#include "engineIO.h"
#include "Stage.h"
#include "AssetManager.h"
#include "EventManager.h"

namespace Amber {

    Engine::Engine(const string& name, int x, int y, int width, int height) :
            assets(new AssetManager()),
            events(new EventManager()),
            stage(new Stage(this, name, x, y, width, height)) {
    }

    Engine::~Engine() {
        delete assets;
        delete events;
        delete stage;
    };

    void Engine::init() {
        stage->init();
//    assets->buildAll();
        events->addHandler(
                window_events::CloseHandler([&](window_events::CloseEvent&) {
                    running = false;
                })
        );
        events->addHandler(
                window_events::FocusHandler([&](window_events::FocusEvent& e) { stage->focused = e.focused; })
        );
        stage->show();

        running = true;
    }

    void Engine::kill() {
        running = false;
    }

    bool Engine::getRunning() const {
        return running;
    }

}