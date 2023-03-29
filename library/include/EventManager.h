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
	struct EventManager {
//		using handlerMap = std::map<std::type_index, IHandlerContainer*>;
		using handlerMap = std::map<int, std::vector<Handler<int>>*>;
		handlerMap handlers;

	public:

		~EventManager();

		std::vector<Handler<int>>& getHandlers(int i) {
			if (!handlers.contains(i))
				handlers.emplace(i, new std::vector<Handler<int>>);
			return *handlers.at(i);
		}

		void addHandler(int i, const Handler<int>& handler) {
			getHandlers(i).push_back(handler);
		}

		void onEvent(int event) {
			if (!handlers.contains(event)) return;
			for (auto& handler: getHandlers(event)) {
				handler(event);
			}
		}

//		template<class T>
//		std::vector<Handler<T>>& getHandlers() {
//			if (!handlers.contains(typeid(T)))
//				handlers.emplace(typeid(T), new HandlerVector<T>);
//			return static_cast<HandlerVector<T>*>(handlers.at(typeid(T)))->handlers;
//		}

//		template<class T>
//		void addHandler(const Handler<T>& handler) {
//			getHandlers<T>().push_back(handler);
//		}

//		template<class T>
//		void onEvent(T& event) {
//			if (!handlers.contains(typeid(T))) return;
//			for (auto& handler: getHandlers<T>()) {
//				handler(event);
//			}
//		}

		void handleType(int i, std::vector<int>* events) {
			auto at = handlers.find(i);
			if  (at != handlers.end()){
				for (auto& h : *at->second)
					for (auto& e : *events)
						h(e);
			}
		}

//		void handleType(std::type_index type, ErasedContainer* erased_vector_events);

	};

}

#endif //ENGINE_EVENTMANAGER_H
