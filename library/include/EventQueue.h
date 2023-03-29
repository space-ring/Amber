//
// Created by croissant on 09/09/2022.
//

#ifndef ENGINETEST_EVENTQUEUE_H
#define ENGINETEST_EVENTQUEUE_H

#include <mutex>
#include <deque>
#include "events.h"
#include "type_erased.h"
#include <map>
#include <typeindex>
#include <set>

namespace Amber {
	struct EventQueue {
		using eventMap = std::map<std::type_index, EventContainer*>;
		eventMap events;
		virtual ~EventQueue() {
			for (auto& s: events) {
				delete s.second;
			}
		}

		std::mutex mutex;

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
