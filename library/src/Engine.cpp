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
			handlers(new EventManager()),
			stage(new Stage(name, x, y, width, height)) {
		std::cout << "engine on thread " << std::this_thread::get_id() << std::endl;
	}

	Engine::~Engine() {
		std::cout << "destroying engine from thread " << std::this_thread::get_id() << std::endl;
		delete assets;
		delete handlers;
		delete stage;
	};

	void Engine::init() {
		stage->init();

		handlers->addHandler(
				window_events::CloseHandler([&](window_events::CloseEvent&) { running = false; })
		);
		handlers->addHandler(
				window_events::FocusHandler([&](window_events::FocusEvent& e) { stage->focused = e.focused; })
		);
		handlers->addHandler(
				window_events::FramebufferSizeHandler([&](window_events::FramebufferSizeEvent& e) {
					glViewport(0, 0, e.width, e.height);
				})
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