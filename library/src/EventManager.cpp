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

	void EventManager::handleType(std::type_index type, ErasedContainer* erased_vector_events) {
		std::cout << "handling type " << type.name() << std::endl;
		auto at = handlers.find(type);
		std::cout << "address: " << &at << std::endl;
		if (at != handlers.end()) {
			handlers.at(type)->handle(erased_vector_events);
		}
	}
}