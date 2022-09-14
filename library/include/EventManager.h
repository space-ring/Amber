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
#include "type_erased.h"

namespace Amber {
	class EventManager {
		using handlerMap = std::map<std::type_index, IErasedHandlable*>;
		handlerMap handlers;

	public:

		~EventManager();

		template<class T>
		std::vector<Handler<T>>& getHandlers() {
			if (!handlers.contains(typeid(T)))
				handlers.emplace(typeid(T), new ErasedHandlerVector<T>);
			return static_cast<ErasedHandlerVector<T>*>(handlers.at(typeid(T)))->handlers;
		}

		template<class T>
		void addHandler(const Handler<T>& handler) {
			getHandlers<T>().push_back(handler);
		}

		template<class T>
		void onEvent(T& event) {
			for (auto& handler: getHandlers<T>()) {
				handler(event);
			}
		}

		void handleType(std::type_index type, type_erased* erased_vector_events);

		void clear(std::type_index type);

	};

}

#endif //ENGINE_EVENTMANAGER_H
