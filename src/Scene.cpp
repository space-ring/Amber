//
// Created by croissant on 25/02/2022.
//

#include "Scene.h"

Scene::Scene() {}

id Scene::addCamera(Camera* camera) {
    static id index = 0;
    cameras->insert(std::pair<id, Camera*>(index, camera));
    return index++;
}

id Scene::addLight(Light* light) {
    static id index = 0;
    lights->insert(std::pair<id, Light*>(index, light));
    return index++;
}

void Scene::removeCamera(id index) {
    cameras->erase(index);
}

void Scene::removeLight(id index) {
    lights->erase(index);
}

