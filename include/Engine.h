//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <map>
#include "Stage.h"
#include "Scene.h"
#include "graphics.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "AssetManager.h"
#include <thread>

//todo default members for resources (vao = 0, etc)
//todo initialise members that don't depend on constructor in class declaration
//todo use references instead of pointers
//todo circular dependencies with IO
class Engine {
    using string = std::string;

private:
    std::map<string, Scene*>* scenes = new std::map<string, Scene*>;
    bool running = false;
    Scene* frontScene = nullptr;

    void init();

    Stage* stage;

public:

    AssetManager* assets = new AssetManager();

    Engine(const string& name, int x, int y, int width, int height);

    /* Make callable for thread launching */
//    void operator()(const string& manifest = "");

    virtual ~Engine();

    /* Initialises all listed resources, scenes and stages. Enters main render loop */
    void run();

    /* Requests for the window to shut down */
    void kill();

    Stage* getStage() const;

    Scene* getFrontScene();

    void addScene(const string& id, Scene* scene);

    Scene* getFront();

};


#endif //ENGINE_ENGINE_H
