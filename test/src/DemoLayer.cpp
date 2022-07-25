//
// Created by croissant on 19/07/2022.
//

#include "scenes/demo/DemoLayer.h"
#include "scenes/demo/DemoScene.h"
#include "Stage.h"

DemoLayer::DemoLayer() :
        onEnter([&](Amber::window_events::EnterEvent& event) {
            r = (float) event.entered;
            glClearColor(r, g, b, a);
        }),
        onFocus([&](Amber::window_events::FocusEvent& event) {
            g = (float) event.focused;
            glClearColor(r, g, b, a);
        }),
        cube(Amber::Model()){
    addHandler(onEnter);
    addHandler(onFocus);
}

void DemoLayer::render() {
    //render all cubes
    Amber::Shader* wire = DemoScene::getInstance().stage->getRoot()->assets->getShader("wire")->build();
    wire->start();

    models.buffer(cube.getMesh());

    glBindVertexArray(cube.getMesh()->getVao());

}

void DemoLayer::update() {

}

void DemoLayer::pick(double x, double y) {

}
