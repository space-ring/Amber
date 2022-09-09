//
// Created by croissant on 19/07/2022.
//

#ifndef ENGINE_STATEBUFFER_H
#define ENGINE_STATEBUFFER_H

#include <mutex>
#include "NoDefaultSingleton.h"

namespace Amber {
	template<class T>
	class StateBuffer : public NoDefaultSingleton<StateBuffer<T>> {
		friend Singleton<StateBuffer<T>>;

		T logicState;
		typename T::R copy, renderState;
		std::mutex copy_mutex;

		template<class... Args>
		StateBuffer(Args... args) : logicState(args...), copy(logicState.getR()), renderState(copy) {}

	public:

		void bufferUpdate() {
			std::lock_guard lock(copy_mutex);
			copy = logicState.getR();
		}

		void bufferCopy() {
			std::lock_guard lock(copy_mutex);
			renderState = copy;
		}

		T& getLogicState() {
			return logicState;
		}

		typename T::R& getRenderState() {
			return renderState;
		}
	};

}
#endif //ENGINE_STATEBUFFER_H
