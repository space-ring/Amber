#include <iostream>
#include <thread>
#include "src/Engine.h"

//todo use glfw library instead of sources (for this test.exe)

using namespace std;

int main() {
    thread t([]() {
        Engine* engine = new Engine("Engine", 100, 100, 500, 500);
        engine->run();
        delete engine;
    });
    t.join();
    cout << "testing" << endl;
    return 0;
}