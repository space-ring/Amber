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
#include "IApplication.h"
#include "synchapi.h"
#include <string>
#include <chrono>

std::chrono::steady_clock::time_point now();

namespace Amber {

	template<class Game>
	class Application : public IApplication {

		StateBuffer<Game> buffer;
		std::jthread gameThread;

		void gameLoop(std::chrono::milliseconds rate) {
			using namespace std::chrono_literals;
			unsigned long long int frames = 0;
			auto start = now();

			game.start();
			while (game.running) {
				auto frame_start = now();
				{
					std::lock_guard lock(q.mutex);
					for (auto& [type, e]: q.events) {
						game.handlers.handleType(type, e);
					}
					q.clearEvents();
				}
				game.update();
				buffer.bufferUpdate();

				++frames;

				std::this_thread::sleep_until(frame_start + rate);
//			while (frame_start + rate - now() > 0ms) {
//			}

				if (now() - start >= 1s) {
					std::cout << "game fps " << frames << std::endl;
					frames = 0;
					start = now();
				}
			}
			std::cout << "game stop" << std::endl;
			engine.kill(); //todo remove
		}

		void renderLoop() {
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
					std::cout << "render fps " << frames << std::endl;
					frames = 0;
					start = std::time(nullptr);
				}
			}

			std::cout << "render stop" << std::endl;
		};

	public:
		Game& game = buffer.getLogicState();
		typename Game::R& R = buffer.getRenderState();

		template<class... Args>
		Application(std::string_view name, int x, int y, int width, int height, Args... args) :
				buffer(args...),
				IApplication(name, x, y, width, height) {
		}

		virtual ~Application() {
			game.running = false;
			if (gameThread.joinable()) gameThread.join();
		}

		void run(std::chrono::milliseconds rate) {
			gameThread = std::jthread{&Application::gameLoop, this, rate};
			renderLoop();
			game.running = false;
			if (gameThread.joinable()) gameThread.join();
		}
	};
}

#endif //ENGINETEST_APPLICATION_H
