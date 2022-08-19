//
// Created by croissant on 11/07/2022.
//

#ifndef ENGINE_LAYER_H
#define ENGINE_LAYER_H

#include "event.h"
#include "ModelManager.h"
#include "EventManager.h"
#include <map>

namespace Amber {
	class Layer {

	protected:
		ModelManager models;

	public:
		EventManager handlers;

		bool active = true;

		Layer() = default;

		virtual ~Layer() = default;

		virtual void build() = 0;

		virtual void show() = 0;

		virtual void hide() = 0;

		virtual void update() = 0;

		virtual Model* pick(int x, int y) = 0;

		virtual void render() = 0;

	};

}


#endif //ENGINE_LAYER_H
