//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include <map>
#include <vector>
#include "Camera.h"
#include "light.h"
#include "Frame.h"
#include "Mesh.h"
#include "Model.h"
#include "variadic.h"

using id = unsigned long;

class Scene {

private:
    std::map<id, Camera*>* cameras = new std::map<id, Camera*>;
    std::map<id, Light*>* lights = new std::map<id, Light*>;
    std::map<Mesh*, std::vector<id>*>* instances = new std::map<Mesh*, std::vector<id>*>;

public:
    Scene();

    id addCamera(Camera* camera);

    id addLight(Light* light);

    void removeCamera(id index);

    void removeLight(id index);
};


#endif //ENGINE_SCENE_H
