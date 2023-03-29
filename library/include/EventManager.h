//
// Created by Chris on 30/06/2022.
//

#ifndef ENGINE_EVENTMANAGER_H
#define ENGINE_EVENTMANAGER_H

#include <map>
#include <vector>
#include "events.h"
#include <typeindex>
#include "type_erased.h"

namespace Amber {
	class EventManager {
		using handlerMap = std::map<std::type_index, HandlerContainer*>;
		handlerMap handlers;

	public:

		~EventManager() {
			for (std::pair s: handlers) {
				delete s.second;
			}
		}

		template<class T>
		std::vector<Handler<T>>& getHandlers() {
			if (!handlers.contains(typeid(T)))
				handlers.emplace(typeid(T), new HandlerVector<T>);
			return static_cast<HandlerVector<T>*>(handlers.at(typeid(T)))->handlers;
		}

		template<class T>
		void addHandler(const Handler<T>& handler) {
			getHandlers<T>().push_back(handler);
		}

		template<class T>
		void onEvent(T& event) {
			if (!handlers.contains(typeid(T))) return;
			for (auto& handler: getHandlers<T>()) {
				handler(event);
			}
		}

		void handleType(std::type_index type, EventContainer* erased_vector_events) {
			auto at = handlers.find(type);
			if (at != handlers.end()) {
				handlers.at(type)->handle(erased_vector_events);
			}
		}
	};

}

#endif //ENGINE_EVENTMANAGER_H
