#include <iostream>
#include "src/Engine.h"


using namespace std;

int main() {
    Engine *engine = new Engine("Engine", 100, 100, 500, 500);
    engine->run();
    delete engine;
    return 0;
}