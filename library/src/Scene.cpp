//
// Created by croissant on 25/02/2022.
//

#include "Scene.h"
#include "Engine.h"
#include "Stage.h"

namespace Amber {
    using id = unsigned long;
    using string = std::string;


    Scene::Scene() {}

    Scene::~Scene() = default;

    id Scene::addCamera(Camera* camera) {
        static id index = 0;
        cameras.insert(std::pair(index, camera));
        return index++;
    }

    id Scene::addLight(Light* light) {
        static id index = 0;
        lights.insert(std::pair(index, light));
        return index++;
    }

    void Scene::removeCamera(id index) {
        cameras.erase(index);
    }

    void Scene::removeLight(id index) {
        lights.erase(index);
    }

    void Scene::render() {
        layers.render();
    }

    void Scene::pick(double x, double y) {
        layers.pick(x, y);
    }

    LayerStack& Scene::getLayers() {
        return layers;
    }

    void Scene::build() {
        if (built) return;
        built = true;
    }

    void Scene::setStage(Stage* stage) {
        Scene::stage = stage;
    }

}