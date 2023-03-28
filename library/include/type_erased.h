//
// Created by croissant on 11/09/2022.
//

#ifndef ENGINETEST_TYPE_ERASED_H
#define ENGINETEST_TYPE_ERASED_H

#include <vector>
#include <typeindex>
#include "events.h"

namespace Amber {
	struct ErasedContainer {
		virtual ~ErasedContainer() = default;

		virtual void clear() = 0;
	};

	template<class T>
	struct Vector : ErasedContainer { //e.g. used for handlers T = Event
		std::vector<T> data;

		void clear() override {
			data.clear();
		}
	};

	struct IHandlerContainer : ErasedContainer {
		virtual ~IHandlerContainer() = default;

		virtual void handle(ErasedContainer* erased_vector) = 0;
	};

	template<class T>
	struct HandlerVector : IHandlerContainer { //e.g. used for events T = Handler<Event>
		std::vector<Handler<T>> handlers;

		void clear() override {
			handlers.clear();
		}

		void handle(ErasedContainer* erased_events) override {
			auto& events = dynamic_cast<Vector<T>*>(erased_events)->data;
			for (auto& event: events) {
				for (auto& handler: handlers) {
					handler(event);
				}
			}
		}
	};
}

#endif //ENGINETEST_TYPE_ERASED_H
