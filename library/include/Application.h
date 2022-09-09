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

namespace Amber {

	template<class Game>
	class Application : public NoDefaultSingleton<Application<Game>> {
		friend Singleton<Application<Game>>;

		StateBuffer<Game>& buffer;
		std::jthread gameThread;
		std::deque<Event> game_events;
		std::mutex event_mutex;

		void gameLoop() {
			game.start();
			while (game.running) {
				std::lock_guard lock(event_mutex);
				for (auto& event: game_events) {
					buffer.getLogicState().handlers.onEvent(event);
				}
				game.update();
				buffer.bufferUpdate();
			}
			std::cout << "game stop" << std::endl;
			Engine::getInstance().kill(); //todo remove
		}

		void renderLoop() {
			Engine& engine = Engine::getInstance();
			engine.init();
			unsigned long long int frames = 0;

			std::cout << "running engine on thread " << std::this_thread::get_id() << std::endl;

			std::time_t start = std::time(nullptr);

			Amber::Stage& stage = Stage::getInstance();

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
		Game& game = buffer.getLogicState();
		typename Game::R& R = buffer.getRenderState();

		template<class... Args>
		Application(const string& name, int x, int y, int width, int height, Args... args) :
				buffer(StateBuffer<Game>::getInstance(args...)) {
			Engine::getInstance(name, x, y, width, height);
		}

		virtual ~Application() {
			game.running = false;
			if (gameThread.joinable()) gameThread.join();
		};

		void run() {
			gameThread = std::jthread{&Application::gameLoop, this};
			renderLoop();
			game.running = false; //todo remove this
			if (gameThread.joinable()) gameThread.join();
		};

		void putEvent(const Event& event) {
			std::lock_guard lock(event_mutex);
			game_events.push_back(event);
		}

	};
}

#endif //ENGINETEST_APPLICATION_H
