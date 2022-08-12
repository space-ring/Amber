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

    void Scene::update() {
        layers.update();
    }

}