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

	void EventManager::handleType(std::type_index type, type_erased* erased_vector_events) {
		if (handlers.contains(type))
			handlers.at(type)->handle(erased_vector_events);
	}

	void EventManager::clear(std::type_index type) {
		if (handlers.contains(type)) handlers.at(type)->clear();
	}
}