//
// Created by croissant on 05/07/2022.
//

#include <iostream>
#include "scenes/demo/DemoScene.h"
#include "Stage.h"


void DemoScene::cameraControl() {
	//up 265
	//left 263
	//down 264
	//right 262
	glm::vec3 direction(0);
	if (keys.down.contains(GLFW_KEY_D))
		direction.x += 1;
	if (keys.down.contains(GLFW_KEY_A))
		direction.x += -1;
	if (keys.down.contains(GLFW_KEY_W))
		direction.y += 1;
	if (keys.down.contains(GLFW_KEY_S))
		direction.y += -1;
	camera.move(direction * cameraSpeed);
}

DemoScene::DemoScene()
		: camera(glm::vec3(0), glm::vec3(0), 85, 500, 500, 0.01, 1000, 0.1, 100) {
	layers.addLayer(&groundLayer);

	handlers.addHandler(Amber::window_events::FramebufferSizeHandler(
			[&](Amber::window_events::FramebufferSizeEvent& e) {
				camera.setAspectRatio((float) e.width, (float) e.height);
			}));

};

DemoScene::~DemoScene() {

}

void DemoScene::build() {
	if (built) return;
	Amber::Engine::getInstance().assets->buildAll();
	Amber::Scene::build();
}

void DemoScene::show() {

}

void DemoScene::hide() {

}

void DemoScene::update() {
	Scene::update();
//	camera.move(glm::vec3(1.0/60, 0, 0));
	cameraControl();
//    std::cout<<camera.position.x << " " << camera.position.y << " " << camera.position.z << " " << std::endl;
//    std::cout<<camera.rotation.x << " " << camera.rotation.y << " " << camera.rotation.z << " " << std::endl;
//    camera.move(glm::vec3(0.0f, 0.0f, -0.1f));
//    camera.rotate(glm::vec3(0, 0, 9.0/60));
}

void DemoScene::pick(int x, int y) {
	Scene::pick(x, y);
}

void DemoScene::render() {
	Scene::render();
}
