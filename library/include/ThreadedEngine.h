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

    template<class Game>
    class ThreadedEngine {

        StateBuffer <Game> buffer;
        std::jthread gameThread;
        std::deque<Event> game_events;
        std::mutex event_mutex;

        void handleEvents() {
            std::lock_guard lock(event_mutex);
            for (auto& event: game_events) {
                buffer.getLogicState().events.onEvent(event);
            }
        }

        void gameLoop() {
            while (game.running) {
                handleEvents();
                game.update();
                buffer.bufferUpdate();
            }
            std::cout << "game stop" << std::endl;
        }

        void renderLoop() {
            engine->init();

            std::time_t start = std::time(nullptr);
            int frames = 0;

            while (engine->getRunning()) {
                buffer.bufferCopy();
                Amber::Stage* stage = engine->getStage();
                stage->update();
                stage->pick();
                stage->poll();
                stage->render();
//                using namespace std::chrono_literals;
//                std::this_thread::sleep_for(5s);

                ++frames;
                std::time_t now = std::time(nullptr);
                if (std::difftime(now, start) >= 1.0) {
                    std::cout << frames << std::endl;
                    frames = 0;
                    start = std::time(nullptr);
                }
            }

            std::cout << "render stop" << std::endl;
            delete engine;
            engine = nullptr;
        }

    public:
        Game& game = buffer.getLogicState();
        Engine* engine;

        void run() {
            gameThread = std::jthread{&ThreadedEngine::gameLoop, this};
            renderLoop();
            game.running = false;
            gameThread.join();
        };

        ThreadedEngine(const string& name, int x, int y, int width, int height)
                : engine(new Engine(name, x, y, width, height)) {};

        virtual ~ThreadedEngine() {
            if (engine) engine->kill();
            game.running = false;
            if (gameThread.joinable()) gameThread.join();
        };

    };
}

#endif //ENGINETEST_THREADEDENGINE_H
