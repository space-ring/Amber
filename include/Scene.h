//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include <string>
#include <map>
#include <vector>
#include "Camera.h"
#include "light.h"
#include "Frame.h"
#include "Mesh.h"
#include "Model.h"
#include "variadic.h"
#include "AssetManager.h"
#include "rendering.h"
#include "ModelManager.h"

//todo static polymorphism? how to store any scene in a vector?
class Scene {
    using id = unsigned long;
    using string = std::string;

protected:

    int width, height;
    std::map<id, Camera*>* cameras = new std::map<id, Camera*>;
    std::map<id, Light*>* lights = new std::map<id, Light*>;
    std::map<Mesh*, std::vector<id>*>* instances = new std::map<Mesh*, std::vector<id>*>;

    Model* last_picked = nullptr;
    Model* dragged = nullptr;
    Model* primed = nullptr;
    Model* last_focused = nullptr;

    const GenericLens wire = DEFAULT_WIRE;
    const GenericLens picker = DEFAULT_PICKER;

public:

    ModelManager* models;

    virtual ~Scene();

    id addCamera(Camera* camera);

    id addLight(Light* light);

    void removeCamera(id index);

    void removeLight(id index);

    virtual void setup() = 0;

    virtual void show() = 0;

    virtual void hide() = 0;

    virtual void render() = 0;
};

#endif //ENGINE_SCENE_H
