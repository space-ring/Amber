//
// Created by croissant on 25/02/2022.
//

#include "Scene.h"
#include "Stage.h"

namespace Amber {

	Scene::Scene(Stage& stage) : stage(stage), layers(*this) {}

	void Scene::show() {
		layers.show();
	}

	void Scene::hide() {
		layers.hide();
	}

	void Scene::update() {
		layers.update();
	}

	void Scene::pick(int x, int y) {
		Model* picked = layers.pick(x, y);
		//todo
	}

	void Scene::render() {
		layers.render();
	}
}