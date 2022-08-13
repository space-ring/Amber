//
// Created by croissant on 11/08/2022.
//

#include "scenes/demo/GroundLayer.h"
#include "scenes/demo/DemoScene.h"
#include "Engine.h"

GroundLayer::GroundLayer() {
//    ground.setMesh(Amber::Engine::getInstance().assets->getMesh("plane"), 10);
//    ground.translate(glm::vec3(0, 0, -5));
//    ground.rotate(glm::vec3(0, 0, 0));
//    ground.m_scale(glm::vec3(2, 2, 2));
//    models.add(ground, 10);
//    bg.setMesh(Amber::Engine::getInstance().assets->getMesh("plane"), 10);
//    bg.translate(glm::vec3(0, 0, -25));
//    bg.m_scale(glm::vec3(30, 30, 10));
//    models.add(bg, 10);

//    int x = 2, y = 2;
//    float hx = (float) x / 2, hy = (float) y/2;
//
//    master.translate(glm::vec3(-hx, -hy, -50));
//    master.scale(glm::vec3(0.5));
//
//    for (int i = 0; i < x; ++i) {
//        for (int j = 0; j < y; ++j) {
//            Amber::Model m(Amber::Engine::getInstance().assets->getMesh("plane"));
//            m.getTransform()->attachParent(master, false);
//            m.translate(glm::vec3(j * 2 - hx, i * 2 - hy, 0));
//            models.add(m, 11000);
//        }
//    }

    Amber::Mesh* mesh = Amber::Engine::getInstance().assets->getMesh("plane");
    m1.setMesh(mesh, 100);
    m2.setMesh(mesh, 100);
    m3.setMesh(mesh, 100);
    m4.setMesh(mesh, 100);

    m2.getTransform()->attachParent(*m1.getTransform(), true);
    m3.getTransform()->attachParent(*m1.getTransform(), true);
    m4.getTransform()->attachParent(*m1.getTransform(), true);

    m1.translate(glm::vec3(-1, -1, -20));
    m2.translate(glm::vec3(2, 0, 0));
    m3.translate(glm::vec3(0, 2, 0));
    m4.translate(glm::vec3(2, 2, 0));
    m1.scale(glm::vec3(2.5));

    models.add(m1, 100);
    models.add(m2, 100);
    models.add(m3, 100);
    models.add(m4, 100);

    ground.setMesh(mesh, 100);
    ground.setRotation(glm::vec3(90, 0, 0));
    ground.setScale(glm::vec3(100));
    ground.setTranslation(glm::vec3(0, -5, 0));

    models.add(ground, 100);

}

void GroundLayer::render() {
    Amber::Engine& engine = Amber::Engine::getInstance();
    DemoScene& scene = DemoScene::getInstance();
    Amber::Mesh* plane = engine.assets->getMesh("plane");

    Amber::Shader* shader = engine.assets->getShader("basic")->start();

    glBindVertexArray(plane->getVao());

    models.buffer(plane);//this buffer should be moved to pick

    glUniformMatrix4fv(14, 1, false, glm::value_ptr(scene.camera.getView()));
    glUniformMatrix4fv(18, 1, false, glm::value_ptr(scene.camera.getPerspective()));

    glDrawElementsInstanced(GL_TRIANGLES, plane->getElementCount(), GL_UNSIGNED_INT, nullptr, models.getCount(plane));

    glBindVertexArray(0);
    shader->stop();
}

Amber::Model* GroundLayer::pick(double x, double y) {
    //todo example of standardised picking (model manager):
    Amber::Engine& engine = Amber::Engine::getInstance();
    DemoScene& scene = DemoScene::getInstance();
    Amber::Mesh* plane = engine.assets->getMesh("plane");

    Amber::Shader* shader = engine.assets->getShader("pick");

    glBindVertexArray(plane->getVao());
    models.buffer(plane); //buffer here only once per frame (in whichever function comes first after bufferCopy

    glUniformMatrix4fv(14, 1, false, glm::value_ptr(scene.camera.getView()));
    glUniformMatrix4fv(18, 1, false, glm::value_ptr(scene.camera.getPerspective()));

    unsigned int count = 4; // this is the number of pickable models from this mesh
    glDrawElementsInstanced(GL_TRIANGLES, plane->getElementCount(), GL_UNSIGNED_INT, nullptr, count);

    glBindVertexArray(0);
    shader->stop();

    return nullptr;
}

void GroundLayer::update() {
//    m1.translate(glm::vec3(0,0.1/60, 0));
//    m1.rotate(glm::vec3(0, 0, 90.0 / 60));
}
