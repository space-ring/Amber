//
// Created by croissant on 19/07/2022.
//

#include "scenes/demo/DemoLayer.h"
#include "scenes/demo/DemoScene.h"
#include "Stage.h"

DemoLayer::DemoLayer() :
        cube(Amber::Model(Amber::Engine::getInstance().assets->getMesh("box"))) {
    cube.setTranslation(glm::vec3(0, 0, -5));
    cube.setScale(glm::vec3(1,1 ,1));
    models.add(cube, 50);
}

void DemoLayer::render() {
    //render all cubes
    DemoScene& scene = DemoScene::getInstance();

    glCheckError();
    Amber::Shader* shader = Amber::Engine::getInstance().assets->getShader("basic");
    Amber::Mesh* cubemesh = cube.getMesh();
    shader->start();

    glCheckError();
    glBindVertexArray(cubemesh->getVao());
    models.buffer(cubemesh);

    glCheckError();
    glUniformMatrix4fv(14, 1, false, glm::value_ptr(scene.camera.getView()));

    glCheckError();
    glUniformMatrix4fv(18, 1, false, glm::value_ptr(scene.camera.getPerspective()));

    glCheckError();
    glDrawElementsInstanced(GL_TRIANGLES, cubemesh->getElementCount(), GL_UNSIGNED_INT, nullptr,
                            models.getCount(cubemesh));
    glCheckError();
    shader->stop();
    glBindVertexArray(0);
}

void DemoLayer::update() {
    cube.rotate(glm::vec3(45.0/60));
}

Amber::Model* DemoLayer::pick(double x, double y) {
    return nullptr;
}
