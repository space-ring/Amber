#include <iostream>
#include <thread>
#include "Engine.h"
#include "event.h"

//todo use glfw library instead of sources (for this test.exe)

using namespace std;

int main() {
    auto *engine = new Engine("Engine", 100, 100, 500, 500);
    engine->stage->addOnFocus([](context_event::focusEvent event) {
        cout << "focused " << event.focused << endl;
    });

    //declare all resources, add them to engine;
    thread t([](Engine *engine) {
        engine->run();
    }, engine);
    t.join();
    delete engine;
    return 0;
}