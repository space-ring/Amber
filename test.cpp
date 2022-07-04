#include <iostream>
#include <thread>
#include "Engine.h"
#include "event.h"
#include "Stage.h"
#include "managers.h"
#include "rendering.h"

//todo use glfw library instead of sources (for this test.exe)


struct C {
    int p;
};

struct A : C {
    int check;
};
struct B : C {
    float none;
};

void call(const A& a) {
    std::cout << a.check << std::endl;
}

int main() {
    auto* engine = new Engine("Engine", 710, 100, 500, 500);

    float r{0}, b{0}, g{0}, a{1};
    
    window_events::EnterHandler h1(
            [&](const window_events::EnterEvent& event) {
                r = (float) event.entered;
                glClearColor(r, g, b, a);
            }
    );

    window_events::FocusHandler h2(
            [&](const window_events::FocusEvent& e) {
                g = (float) e.focused;
                glClearColor(r, g, b, a);
            }
    );
    engine->assets->addManifest("test/manifest");

    engine->events->addHandler(h1);
    engine->events->addHandler(h2);

    std::thread t([](Engine* engine) {
        engine->run();
        delete engine; //GL context exists only in this thread
    }, engine);

    /*
     * ..........................................
     * Game logic here
     */

    // program close
    t.join();
    return 0;
}