//
// Created by croissant on 11/09/2022.
//

#ifndef ENGINETEST_TYPE_ERASED_H
#define ENGINETEST_TYPE_ERASED_H

#include <vector>
#include <typeindex>
#include "events.h"

namespace Amber {

	struct EventContainer {
		virtual ~EventContainer() = default;
	};

	template<class T>
	struct EventVector : EventContainer { //e.g. used for handlers T = Event
		std::vector<T> events;
	};

	struct HandlerContainer {
		virtual void handle(EventContainer* erased_vector) = 0;
	};

	template<class T>
	struct HandlerVector : HandlerContainer { //e.g. used for events T = Handler<Event>
		std::vector<Handler<T>> handlers;

		void handle(EventContainer* erased_events) override {
			auto& events = dynamic_cast<EventVector<T>&>(*erased_events).events;
			for (auto& event: events) {
				for (auto& handler: handlers) {
					handler(event);
				}
			}
		}
	};
}

#endif //ENGINETEST_TYPE_ERASED_H
