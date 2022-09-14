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
//		using handlerMap = std::map<unsigned long long int, std::vector<GenericHandler>*>;
		using handlerMap = std::map<std::type_index, type_erased*>;
		handlerMap handlers;

	public:

		~EventManager();

		template<class T>
		std::vector<Amber::Handler<T>>& getHandlers() {
			if (!handlers.contains(typeid(T)))
				handlers.emplace(typeid(T), new ErasedVector<Amber::Handler<T>>);
			return static_cast<ErasedVector<Handler<T>>*>(handlers.at(typeid(T)))->data;
		}

		// add any derived handler
//		template<class T>
//		void addHandler(const EventHandler <T>& handler) {
//			if (!handlers->contains(handler.type))
//				(*handlers)[handler.type] = new std::vector<GenericHandler>;
//
//			auto* list = handlers->at(handler.type);
//			//upcast to change template parameter (T is any here)
//			list->push_back(EventHandler<T>::upcast(handler));
//		}

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

//		template<class T>
//		void addHandler(const EventHandler <T>& handler) {
//			if (!handlers->contains(typeid(T)))
//				(*handlers)[typeid(T)] = new std::vector<GenericHandler>;
//
//			auto* list = handlers->at(typeid(T));
//			//upcast to change template parameter (T is any here)
//			list->push_back(EventHandler<T>::upcast(handler));
//		}

		void clearHandlers(unsigned long id);

//		template<class T>
//		void onEvent(T& event) {
//			auto id = EventHandler<T>::type;
//			if (!handlers->contains(id)) return;
//			for (auto& handler: *handlers->at(id)) {
//				handler(event);
//			}
//		}

//		void onEvent(Event& event) {
//			if (!handlers->contains(typeid(event))) return;
//			for (auto& handler: *handlers->at(typeid(event))) {
//				handler(event);
//			}
//		}
	};

}

#endif //ENGINE_EVENTMANAGER_H
