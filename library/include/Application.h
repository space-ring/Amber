//
// Created by croissant on 19/07/2022.
//

#ifndef ENGINETEST_APPLICATION_H
#define ENGINETEST_APPLICATION_H

#include <mutex>
#include <thread>
#include "StateBuffer.h"
#include "Engine.h"
#include "Stage.h"
#include "snake.h"
#include "IApplication.h"

namespace Amber {

	template<class Game>
	class Application : public IApplication {

		void gameLoop(std::chrono::milliseconds rate) override {
			game.start();
			while (game.running) {
				std::lock_guard lock(mutex);
				for (auto& [type, e]: events) {
					game.handlers.handleType(type, e);
					clearEvents(type);
				}
				game.update();
				buffer.bufferUpdate();
				std::this_thread::sleep_for(rate); //todo this is buggy, sometimes takes longer than rate
			}
			std::cout << "game stop" << std::endl;
			engine.kill(); //todo remove
		}

		void renderLoop() override {
			engine.init();
			unsigned long long int frames = 0;

			std::cout << "running engine on thread " << std::this_thread::get_id() << std::endl;

			std::time_t start = std::time(nullptr);

			Amber::Stage& stage = engine.stage;

			while (engine.getRunning()) {
				buffer.bufferCopy();
				stage.update();
				stage.pick();
				stage.poll();
				stage.render();

				++frames;
				std::time_t now = std::time(nullptr);
				if (std::difftime(now, start) >= 1.0) {
					std::cout << frames << std::endl;
					frames = 0;
					start = std::time(nullptr);
				}
			}

			std::cout << "render stop" << std::endl;
		}

	public:
		StateBuffer<Game> buffer;
		Engine engine;
		Game& game = buffer.getLogicState();
		std::chrono::milliseconds rate;
		typename Game::R& R = buffer.getRenderState();

		template<class... Args>
		Application(const string& name, int x, int y, int width, int height, std::chrono::milliseconds rate,
		            Args... args) :
				rate(rate),
				buffer(args...),
				engine(*this, name, x, y, width, height) {
		}

		virtual ~Application() {
			game.running = false;
			if (gameThread.joinable()) gameThread.join();
		}

		void run() {
			gameThread = std::jthread{&Application::gameLoop, this, rate};
			renderLoop();
			game.running = false; //todo remove this
			if (gameThread.joinable()) gameThread.join();
		}
	};
}

#endif //ENGINETEST_APPLICATION_H
