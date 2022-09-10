//
// Created by Chris on 30/06/2022.
//

#ifndef ENGINE_EVENTMANAGER_H
#define ENGINE_EVENTMANAGER_H

#include <map>
#include <vector>
#include "events.h"
#include <typeinfo>
#include <typeindex>

namespace Amber {
	class EventManager {
		using handlerMap = std::map<std::type_index, std::vector<GenericHandler>*>;
		handlerMap* handlers = new handlerMap();

	public:
		~EventManager();

		// add any handler
		template<class T>
		void addHandler(const EventHandler <T>& handler) {
			if (!handlers->contains(typeid(T)))
				(*handlers)[typeid(T)] = new std::vector<GenericHandler>;

			auto* list = handlers->at(typeid(T));
			//upcast to change template parameter (T is any here)
			list->push_back(EventHandler<T>::upcast(handler));
		}

		void clearHandlers(unsigned long id);

		void onEvent(Event& event) {
			if (!handlers->contains(typeid(event))) return;
			for (auto& handler: *handlers->at(typeid(event))) {
				handler(event);
			}
		}
	};

}

#endif //ENGINE_EVENTMANAGER_H
