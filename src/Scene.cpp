//
// Created by croissant on 25/02/2022.
//

#include "Scene.h"

using id = unsigned long;
using string = std::string;

id Scene::addCamera(Camera* camera) {
    static id index = 0;
    cameras->insert(std::pair(index, camera));
    return index++;
}

id Scene::addLight(Light* light) {
    static id index = 0;
    lights->insert(std::pair(index, light));
    return index++;
}

void Scene::removeCamera(id index) {
    cameras->erase(index);
}

void Scene::removeLight(id index) {
    lights->erase(index);
}

Scene::~Scene() {
    cameras->clear();
    lights->clear();
    delete cameras;
    delete lights;
    delete models;
}

