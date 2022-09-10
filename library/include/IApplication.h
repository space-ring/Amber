//
// Created by croissant on 09/09/2022.
//

#ifndef ENGINETEST_IAPPLICATION_H
#define ENGINETEST_IAPPLICATION_H

#include <thread>
#include <deque>
#include <mutex>
#include "events.h"
#include "IMessage.h"

namespace Amber {
	class IApplication : public IMessage {

	protected:
		std::jthread gameThread;

		virtual void gameLoop() = 0;

		virtual void renderLoop() = 0;
	};
}

#endif //ENGINETEST_IAPPLICATION_H
