//
// Created by croissant on 19/07/2022.
//

#ifndef ENGINE_STATEBUFFER_H
#define ENGINE_STATEBUFFER_H

#include <mutex>

namespace Amber {
	template<class T>
	class StateBuffer {
		T logicState;
		typename T::R copy, renderState;
		std::mutex copy_mutex;

	public:

		StateBuffer(T game) : logicState(game), copy(game.getRenderComponent()), renderState(copy) {}

		void bufferUpdate() {
			std::lock_guard lock(copy_mutex);
			copy = logicState;
		}

		void bufferCopy() {
			std::lock_guard lock(copy_mutex);
			renderState = copy;
		}

		T& getLogicState() {
			return logicState;
		}
	};

}
#endif //ENGINE_STATEBUFFER_H
