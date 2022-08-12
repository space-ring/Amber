#include <iostream>
#include <thread>
#include "Engine.h"
#include "Sandbox.h"
#include "scenes/demo/DemoScene.h"
#include "ThreadedEngine.h"

int main() {

    std::cout << "main thread " << std::this_thread::get_id() << std::endl;

    // 1030,470

    Amber::ThreadedEngine<Sandbox> app("Test game", 1030, 470, 500, 500);
    Amber::Engine& engine = Amber::Engine::getInstance();
    Sandbox& game = app.game;

    engine.assets->addManifest("assets/manifest");

    auto plane = engine.assets->getMesh("plane");
    engine.stage->addScene("demo", &DemoScene::getInstance());
    engine.stage->setFrontScene("demo");

    app.run();

    // program close
    return 0;
}