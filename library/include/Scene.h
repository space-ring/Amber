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
#include "ModelTransform.h"
#include "variadic.h"
#include "AssetManager.h"
#include "rendering.h"
#include "ModelManager.h"
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

		using id = unsigned long;
		using string = std::string;

		bool built = false;
		LayerStack layers;
		EventManager handlers;
		ModelManager models;

		ModelTransform* last_picked = nullptr;
		ModelTransform* dragged = nullptr;
		ModelTransform* primed = nullptr;
		ModelTransform* last_focused = nullptr;

	public:

		Stage* stage;

		KeyRegistry keys;

		Scene();

		virtual ~Scene();

		virtual void build() = 0;

		virtual void show() = 0;

		virtual void hide() = 0;

		virtual void update() = 0;

		virtual void pick(int x, int y) = 0;

		virtual void render() = 0;

		void onEvent(Event& event) {
			layers.onEvent(event);
			if (static_cast<Event&>(event).handled) return;
			handlers.onEvent(event);
		}

		void onEvent(window_events::KeyEvent& event){
			if (event.action == 1){
				keys.pressed.insert(event.key);
			} else if (event.action == 0){
				keys.down.erase(event.key);
				keys.released.insert(event.key);
			}
			layers.onEvent(event);
			if (static_cast<Event&>(event).handled) return;
			handlers.onEvent(event);
		}

		LayerStack& getLayers();
	};
}
#endif //ENGINE_SCENE_H
