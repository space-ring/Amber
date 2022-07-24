#include <iostream>
#include <thread>
#include "Engine.h"
#include "event.h"
#include "Stage.h"
#include "rendering.h"
#include "Sandbox.h"
#include "AssetManager.h"
#include "EventManager.h"
#include "StateBuffer.h"
#include "scenes/demo/DemoScene.h"
#include "ThreadedEngine.h"

int main() {

    Amber::ThreadedEngine<Sandbox> app("Test game", 710, 100, 500, 500);
    Amber::Engine* engine = app.engine;
    Sandbox& game = app.game;

//    auto* game = new Sandbox();
//    auto* engine = new Amber::Engine("Engine", 710, 100, 500, 500);

    engine->assets->addManifest("assets/manifest");
    engine->getStage()->addScene("demo", &DemoScene::getInstance());
    engine->getStage()->setFrontScene("demo");

    engine->events->addHandler(
            Amber::EventHandler<Amber::window_events::KeyEvent>(
                    [](Amber::window_events::KeyEvent& event) {
                        std::cout << event.action << " " << (char) event.key << std::endl;
                    }
            )
    );
    using namespace Amber;
    engine->events->addHandler(Amber::EventHandler<Amber::window_events::MotionEvent>(
            [](Amber::window_events::MotionEvent& event) {
                std::cout << "handle " << event.xpos << " " << event.ypos << std::endl;
            })
    );

//    engine->game_events->addHandler(Amber::window_events::FocusHandler(
//            [&](Amber::window_events::FocusEvent& e) {
//                if (!e.focused) engine->kill();
//            })
//    );

    app.run();

//    std::thread t([](Amber::Engine* engine) {
//        engine->run();
//        delete engine; //GL context exists only in this thread
//    }, engine);

    /*
     * ..........................................
     * Game logicState here
     */

    // program close
    return 0;
}