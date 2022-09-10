//
// Created by Chris on 30/06/2022.
//

#include "EventManager.h"

namespace Amber {
	void EventManager::clearHandlers(unsigned long id) {
		//todo
	}

	EventManager::~EventManager() {
		for (std::pair s: *handlers) {
			delete s.second;
		}
		delete handlers;
	}
}