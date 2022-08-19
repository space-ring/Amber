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
#include "event.h"

namespace Amber {

	struct KeyRegistry {
		std::set<int> down;
	};

	class Scene {
	protected:

		using id = unsigned long;
		using string = std::string;

		bool built = false;
		LayerStack layers;
		EventManager handlers;

		ModelTransform* last_picked = nullptr;
		ModelTransform* dragged = nullptr;
		ModelTransform* primed = nullptr;
		ModelTransform* last_focused = nullptr;

	public:

		KeyRegistry keys;

		Scene();

		virtual ~Scene();

		virtual void build() = 0;

		virtual void show() = 0;

		virtual void hide() = 0;

		virtual void update() = 0;

		virtual void pick(int x, int y) = 0;

		virtual void render() = 0;

		template<class T>
		void onEvent(T& event) {
			layers.template onEvent(event);
			if (static_cast<Event&>(event).handled) return;
			handlers.template onEvent(event);
		}

		void onEvent(window_events::KeyEvent& e){
			if (e.action == 1 && !keys.down.contains(e.key)){
				keys.down.insert(e.key);
			} else if (e.action == 0){
				keys.down.erase(e.key);
			}
			layers.template onEvent(e);
			handlers.template onEvent(e);
		}

		LayerStack& getLayers();
	};
}
#endif //ENGINE_SCENE_H
