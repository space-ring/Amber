#include <iostream>
#include <thread>
#include "Engine.h"
#include "event.h"
#include "Stage.h"
#include "managers.h"
#include "rendering.h"
#include "Model.h"

//todo use glfw library instead of sources (for this test.exe)


int main() {

    ModelTransform parent(glm::vec3(10), glm::vec3(0), glm::vec3(1));
    ModelTransform child;
    ModelTransform kid;
    kid.attachParent(child);
    kid.translate(glm::vec3(50));
    child.attachParent(parent);
    parent.translate(glm::vec3(2));
    child.translate(glm::vec3(-3));
    kid.detachParent(true);

    std::vector<Vertex> vertex;
    std::vector<unsigned int> index;

    Mesh mesh(vertex, index);
    ModelTransform transform(glm::vec3(10), glm::vec3(0), glm::vec3(1));

    ModelManager manager;
    Model model(&mesh, transform);
    model.translate(glm::vec3(10));

    manager.add(model);

//    glm::mat4* mat4 = &manager.instances.at(&mesh)->at(0);

    model.setTranslation(glm::vec3(-4));
    manager.remove(model);
    return 0;

    auto* engine = new Engine("Engine", 710, 100, 500, 500);

    float r{0}, b{0}, g{0}, a{1};

    window_events::EnterHandler h1(
            [&](const window_events::EnterEvent& event) {
                r = (float) event.entered;
                glClearColor(r, g, b, a);
            }
    );

    window_events::FocusHandler h2(
            [&](const window_events::FocusEvent& e) {
                g = (float) e.focused;
                glClearColor(r, g, b, a);
            }
    );
    engine->assets->addManifest("test/manifest");

    engine->events->addHandler(h1);
    engine->events->addHandler(h2);

    std::thread t([](Engine* engine) {
        engine->run();
        delete engine; //GL context exists only in this thread
    }, engine);

    /*
     * ..........................................
     * Game logic here
     */

    // program close
    t.join();
    return 0;
}