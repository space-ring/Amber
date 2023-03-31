//
// Created by croissant on 09/09/2022.
//

#ifndef ENGINETEST_EVENTQUEUE_H
#define ENGINETEST_EVENTQUEUE_H

#include <mutex>
#include "events.h"
#include "type_erased.h"
#include <map>
#include <typeindex>

namespace Amber {
	struct EventQueue {
		std::map<std::type_index, EventContainer*> events;
		std::mutex mutex;

		virtual ~EventQueue() {
			for (auto& s: events) {
				delete s.second;
			}
		}

		template<class T>
		std::vector<T>& getEvents() {
			if (!events.contains(typeid(T))) {
				events.emplace(typeid(T), new EventVector<T>);
			}
			return static_cast<EventVector<T>*>(events.at(typeid(T)))->events;
		}

		template<class T>
		void putEvent(const T& event) {
			std::lock_guard lock(mutex);
			getEvents<T>().push_back(event);
		}

		void clearEvents();
	};
}

#endif //ENGINETEST_EVENTQUEUE_H
