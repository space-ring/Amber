//
// Created by croissant on 05/07/2022.
//

#include <iostream>
#include "scenes/demo/DemoScene.h"
#include "Mesh.h"
#include "Stage.h"

DemoScene::DemoScene() {
    layers.addLayer(&demoLayer);
};

DemoScene::~DemoScene() {

}

void DemoScene::build() {
    if (built) return;
    built = true;
    demoLayer.cube.setMesh(stage->getRoot()->assets->getMesh("cube"), 10);
    demoLayer.models.add(demoLayer.cube, 10);
}

void DemoScene::show() {

}

void DemoScene::hide() {

}

void DemoScene::render() {
    Scene::render();
}

void DemoScene::update() {

}

void DemoScene::pick(double x, double y) {
    Scene::pick(x, y);
}
