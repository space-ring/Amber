#include <iostream>
#include <thread>
#include "Engine.h"
#include "event.h"
#include "Stage.h"
#include "managers.h"

//todo use glfw library instead of sources (for this test.exe)

int main() {
    auto* engine = new Engine("Engine", 710, 100, 500, 500);

    float r{0}, b{0}, g{0}, a{1};

    window_event::EnterHandler h1(
            [&](const window_event::EnterEvent& event) {
                r = (float) event.entered;
                glClearColor(r, g, b, a);
            }
    );

    window_event::FocusHandler h2(
            [&](const window_event::FocusEvent& e) {
                g = (float) e.focused;
                glClearColor(r, g, b, a);
                engine->getStage()->focused = e.focused;
            }
    );

    engine->assets->addManifest("test/manifest");

    engine->events->onEvent(window_event::EnterEvent{0, 10});

    engine->events->addHandler(h1);
    engine->events->addHandler(h2);

    std::thread t([](Engine* engine) {
        engine->run();
    }, engine);

    /*
     * ..........................................
     * Game logic here
     */

    // program close
    t.join();
    delete engine;
    return 0;
}