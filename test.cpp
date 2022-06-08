#include <iostream>
#include <thread>
#include "Engine.h"
#include "event.h"
#include "engineIO.h"
#include "Camera.h"
#include "engineIO.h"

//todo use glfw library instead of sources (for this test.exe)
using namespace std;

int main() {

    auto* engine = new Engine("Engine", 100, 100, 500, 500);
    engine->assets->addManifest("test/manifest");
    //declare all resources, add them to engine;
    thread t([](Engine* engine) {
        engine->run();
    }, engine);


    cout << endl;

    // game logic
    // program close
    t.join();
    delete engine;
    return 0;
}