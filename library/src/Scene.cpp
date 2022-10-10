//
// Created by croissant on 25/02/2022.
//

#include "Scene.h"
#include "Engine.h"
#include "Stage.h"

namespace Amber {
	using id = unsigned long;
	using string = std::string;


	Scene::Scene() : layers(*this) {}

	void Scene::build() {
		if (built) return;
		built = true;
		layers.build();
	}

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

	LayerStack& Scene::getLayers() {
		return layers;
	}

}