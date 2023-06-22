//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include <string>
#include <map>
#include <vector>
#include "Camera.h"
#include "light.h"
#include "Frame.h"
#include "Mesh.h"
#include "Transform.h"
#include "variadic.h"
#include "AssetManager.h"
#include "rendering.h"
#include "InstanceBuffer.h"
#include "LayerStack.h"
#include <set>
#include "events.h"

namespace Amber {

	class Stage;

	struct KeyRegistry {
		std::set<int> pressed;
		std::set<int> down;
		std::set<int> released;
	};

	class Scene {
	protected:

		LayerStack layers;
		EventManager handlers;
		InstanceBuffer models;

		Transform* lastPicked = nullptr;
		Transform* dragged = nullptr;
		Transform* primed = nullptr;
		Transform* lastFocused = nullptr;

		Stage& stage;

		KeyRegistry keys;

	public:

		explicit Scene(Stage& stage);

		virtual ~Scene() = default;

		virtual void show() = 0;

		virtual void hide() = 0;

		virtual void update() = 0;

		virtual void pick(int x, int y) = 0;

		virtual void render() = 0;

		template<class T>
		void onEvent(T& event) {
			layers.onEvent(event);
			if (event.handled) return;
			handlers.onEvent(event);
		}

	};

//	template<>
//	void Scene::onEvent<window_events::KeyEvent>(window_events::KeyEvent& event) {
//		if (event.action == 1){
//			keys.pressed.insert(event.key);
//		} else if (event.action == 0){
//			keys.down.erase(event.key);
//			keys.released.insert(event.key);
//		}
//		layers.onEvent(event);
//		if (event.handled) return;
//		handlers.template onEvent(event);
//	}
}
#endif //ENGINE_SCENE_H
