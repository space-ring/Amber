//
// Created by croissant on 19/07/2022.
//

#ifndef ENGINETEST_APPLICATION_H
#define ENGINETEST_APPLICATION_H

#include <mutex>
#include <thread>
#include "StateBuffer.h"
#include "EventQueue.h"
#include "Stage.h"
#include "synchapi.h"
#include <string>
#include <chrono>
#include "Scene.h"

inline std::chrono::steady_clock::time_point now() {
	return std::chrono::steady_clock::now();
}

namespace Amber {

	template<class Game>
	class Application {

		std::jthread gameThread;

	protected:
		EventQueue eventsFromLogic;
		EventQueue eventsFromRenderer;
		Game game;
		Stage stage;

	private:
		void gameLoop(std::chrono::milliseconds rate) {
			using namespace std::chrono_literals;
			unsigned long long int frames = 0;
			auto start = now();

			game.start();
			while (game.running) {
				auto frame_start = now();
				{
					std::lock_guard lock(eventsFromRenderer.mutex);
					for (auto& [type, e]: eventsFromRenderer.events) {
						game.handlers.handleType(type, e);
					}
					eventsFromRenderer.clearEvents();
				}
				game.update();

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
		}

		void renderLoop() {
			unsigned long long int frames = 0;

			std::cout << "running engine on thread " << std::this_thread::get_id() << std::endl;

			std::time_t start = std::time(nullptr);

			stage.show();

			while (stage.isRunning()) {

				{
					std::lock_guard(eventsFromLogic.mutex);
					for (auto& [type, e]: eventsFromLogic.events) {
						stage.handlers.handleType(type, e);
						stage.front.handlers.handleType(type, e);
					}
					eventsFromLogic.clearEvents();
				}
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

		template<class... Args>
		Application(Scene& frontScene, std::string_view manifest, std::string_view name, int x, int y, int width,
		            int height, Args... args) :
				game(eventsFromLogic, args...),
				stage(frontScene, eventsFromRenderer, manifest, name, x, y, width, height) {
		}

		virtual ~Application() {
			game.running = false;
			if (gameThread.joinable()) gameThread.join();
		}

		Application(const Application&) = delete;

		Application(Application&&) = delete;

		Application& operator=(const Application&) = delete;

		Application& operator=(Application&&) = delete;

		void run(std::chrono::milliseconds rate) {
			gameThread = std::jthread{&Application::gameLoop, this, rate};
			renderLoop();
			game.running = false;
			if (gameThread.joinable()) gameThread.join();
		}
	};
}

#endif //ENGINETEST_APPLICATION_H
