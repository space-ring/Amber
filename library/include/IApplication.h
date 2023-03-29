//
// Created by croissant on 09/09/2022.
//

#ifndef ENGINETEST_IAPPLICATION_H
#define ENGINETEST_IAPPLICATION_H

#include <thread>
#include <deque>
#include <mutex>
#include "events.h"
#include "EventQueue.h"
#include <chrono>

namespace Amber {
	class IApplication : public EventQueue {

	protected:
		std::jthread gameThread;

		virtual void gameLoop(std::chrono::milliseconds rate) = 0;

		virtual void renderLoop() = 0;

	public:
		virtual ~IApplication() = default;
	};
}

#endif //ENGINETEST_IAPPLICATION_H
