//
// Created by croissant on 05/07/2022.
//

#include <iostream>
#include "scenes/demo/DemoScene.h"
#include "Stage.h"

DemoScene::DemoScene()
        : camera(glm::vec3(0), glm::vec3(0), 85, 500,500, 0.01, 100, 0.1, 100) {
    layers.addLayer(&demoLayer);
    layers.addLayer(&groundLayer);

    handlers.addHandler(Amber::window_events::FramebufferSizeHandler (
            [&](Amber::window_events::FramebufferSizeEvent& e) {
                camera.setAspectRatio((float) e.width, (float) e.height);
            }));
};

DemoScene::~DemoScene() {

}

void DemoScene::build() {
    if (built) return;
    built = true;
    Amber::Engine::getInstance().assets->buildAll();
}

void DemoScene::show() {

}

void DemoScene::hide() {

}

void DemoScene::render() {
    Scene::render();
}

void DemoScene::update() {
    Scene::update();

//    std::cout<<camera.position.x << " " << camera.position.y << " " << camera.position.z << " " << std::endl;
//    std::cout<<camera.rotation.x << " " << camera.rotation.y << " " << camera.rotation.z << " " << std::endl;
//    camera.move(glm::vec3(0.0f, 0.0f, -0.1f));
//    camera.rotate(glm::vec3(0, 0, 9.0/60));
}

void DemoScene::pick(int x, int y) {
    Scene::pick(x, y);
}
