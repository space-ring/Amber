//
// Created by croissant on 05/07/2022.
//

#include <iostream>
#include "scenes/demo/DemoScene.h"

DemoScene::DemoScene() {
    layers.addLayer(&demoLayer);
};

DemoScene::~DemoScene() {
};

void DemoScene::build() {

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

}
