//
// Created by croissant on 05/07/2022.
//

#include <iostream>
#include "scenes/demo/DemoScene.h"
#include "Stage.h"


void DemoScene::cameraControl() {
	glm::vec3 direction(0);
	if (keys.down.contains(GLFW_KEY_D))
		direction.x += 1;
	if (keys.down.contains(GLFW_KEY_A))
		direction.x += -1;
	if (keys.down.contains(GLFW_KEY_W))
		direction.y += 1;
	if (keys.down.contains(GLFW_KEY_S))
		direction.y += -1;
	if (keys.down.contains(GLFW_KEY_Q))
		direction.z += -1;
	if (keys.down.contains(GLFW_KEY_E))
		direction.z += 1;
	camera.move(direction * cameraSpeed);

	if (keys.down.contains(GLFW_KEY_Z)) {
		groundLayer.active = false;
	} else if (keys.down.contains(GLFW_KEY_X)) {
		groundLayer.active = true;
	}
}

DemoScene::DemoScene()
		: camera(glm::vec3(0), glm::vec3(0), 85, 500, 500, 0.01, 1000, 0.01, 100), Scene() {
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
	stage->engine.assets.buildAll();
	Amber::Scene::build();
}

void DemoScene::show() {

}

void DemoScene::hide() {

}

void DemoScene::update() {
	Scene::update();
	cameraControl();
}

void DemoScene::pick(int x, int y) {
	Scene::pick(x, y);
}

void DemoScene::render() {
	Scene::render();
}
