//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <map>
#include "Scene.h"
#include "graphics.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include <thread>
#include <mutex>
#include <queue>
#include "StateBuffer.h"
#include "EventManager.h"
#include "AssetManager.h"

//todo default members for resources (VAO = 0, etc)
//todo initialise members that don't depend on constructor in class declaration ^^^
//todo use references instead of pointers
//todo circular dependencies with IO
//todo make everything public?
//todo singleton engine?
//todo move semantics (engine should own almost everything)
//todo destructor calls destroy (so can terminate graphics manually)
//todo some vectors need to be changed to lists when random access not needed
namespace Amber {
    class Stage;

    class Engine {
        using string = std::string;

    private:
        volatile bool running = false;
        Stage* stage;

    public:

        AssetManager* assets;
        EventManager* events;
        Engine(const string& name, int x, int y, int width, int height);

        virtual ~Engine();

        /* Initialises all listed resources, scenes and stages. Enters main renderState loop */
        void init();

        /* Requests for the window to shut down */
        void kill();

        Stage* getStage() const {
            return stage;
        };

        bool getRunning() const;

    };

}

#endif //ENGINE_ENGINE_H
