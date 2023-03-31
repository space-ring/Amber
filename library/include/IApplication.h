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
#include "Engine.h"
#include <chrono>

namespace Amber {
	struct IApplication {
		Engine engine;
		EventQueue q;

		IApplication(std::string_view name, int x, int y, int width, int height)
				: engine(*this, name, x, y, width, height) {}
	};
}

#endif //ENGINETEST_IAPPLICATION_H
