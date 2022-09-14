//
// Created by Chris on 30/06/2022.
//

#include "EventManager.h"

namespace Amber {

	EventManager::~EventManager() {
		for (std::pair s: handlers) {
			delete s.second;
		}
	}
}