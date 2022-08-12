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
        cube(Amber::Model(Amber::Engine::getInstance().assets->getMesh("box"))),
        triangle(Amber::Model(Amber::Engine::getInstance().assets->getMesh("triangle"))) {
//    addHandler(onEnter);
//    addHandler(onFocus);
    cube.setTranslation(glm::vec3(0, 0, 10));
    triangle.translate(glm::vec3(0, 0, -5));
    models.add(cube, 50);
}

void DemoLayer::test() {

    Amber::Scene& scene = DemoScene::getInstance();
    Amber::Shader* shader = Amber::Engine::getInstance().assets->getShader("easy");
    shader->start();

    glCheckError();
    glBindVertexArray(triangle.getMesh()->getVao());
    glCheckError();
    glad_glDrawArrays(GL_TRIANGLES, 0, 3);
    glCheckError();
    glBindVertexArray(0);
    shader->stop();
}

void DemoLayer::render() {
    return;
    //render all cubes
    DemoScene& scene = DemoScene::getInstance();

    glCheckError();
    Amber::Shader* shader = Amber::Engine::getInstance().assets->getShader("basic");
    shader->start();

    glCheckError();
    glBindVertexArray(cube.getMesh()->getVao());
    models.buffer(cube.getMesh());

    glCheckError();
    glUniformMatrix4fv(14, 1, false, glm::value_ptr(scene.camera.getView()));

    glCheckError();
    glUniformMatrix4fv(18, 1, false, glm::value_ptr(scene.camera.getPerspective()));

    glCheckError();
    glDrawElementsInstanced(GL_TRIANGLES, cube.getMesh()->getElementCount(), GL_UNSIGNED_INT, nullptr,
                            models.instances.at(cube.getMesh())->size());
    glCheckError();
    shader->stop();
    glBindVertexArray(0);
}

void DemoLayer::update() {
}

void DemoLayer::pick(double x, double y) {

}
