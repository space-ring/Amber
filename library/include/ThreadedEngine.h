//
// Created by croissant on 19/07/2022.
//

#ifndef ENGINETEST_THREADEDENGINE_H
#define ENGINETEST_THREADEDENGINE_H

#include <mutex>
#include <thread>
#include "StateBuffer.h"
#include "Engine.h"
#include "Stage.h"

namespace Amber {

	//todo make this singleton too
	//todo merge with engine?
	template<class Game>
	class ThreadedEngine {

		StateBuffer<Game> buffer;
		std::jthread gameThread;
		std::deque<Event> game_events;
		std::mutex event_mutex;

		void gameHandleEvents() {
			std::lock_guard lock(event_mutex);
			for (auto& event: game_events) {
				buffer.getLogicState().handlers.onEvent(event);
			}
		}

		void gameLoop() {
			while (game.running) {
				gameHandleEvents();
				game.update();
				buffer.bufferUpdate();
			}
			std::cout << "game stop" << std::endl;
		}

		void renderLoop() {
			Engine& engine = Engine::getInstance();
			engine.init();
			unsigned long long int frames = 0;

			std::cout << "running engine on thread " << std::this_thread::get_id() << std::endl;

			std::time_t start = std::time(nullptr);

			Amber::Stage* stage = engine.stage;

			while (engine.getRunning()) {
				buffer.bufferCopy();
				stage->update();
				stage->pick();
				stage->poll();
				stage->render();

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

		void run() {
			gameThread = std::jthread{&ThreadedEngine::gameLoop, this};
			renderLoop();
			game.running = false; //todo remove this
			if (gameThread.joinable()) gameThread.join();
		};

		ThreadedEngine(const string& name, int x, int y, int width, int height) {
			Engine::getInstance(name, x, y, width, height);
		};

		virtual ~ThreadedEngine() {
			game.running = false;
			if (gameThread.joinable()) gameThread.join();
		};

	};
}

#endif //ENGINETEST_THREADEDENGINE_H
