//
// Created by croissant on 11/09/2022.
//

#ifndef ENGINETEST_TYPE_ERASED_H
#define ENGINETEST_TYPE_ERASED_H

#include <vector>
#include <typeindex>

namespace Amber {
	struct type_erased {
		virtual ~type_erased() = default;

		virtual void clear() = 0;
	};

	template<class T>
	struct ErasedVector : type_erased { //e.g. used for handlers T = Event
		std::vector<T> data;

		void clear() override {
			data.clear();
		}
	};

	struct IErasedHandlable : type_erased {
		virtual void handle(type_erased* ErasedVector) = 0;
	};

	template<class T>
	struct ErasedHandlerVector : IErasedHandlable { //e.g. used for events T = Handler<Event>
		std::vector<Handler<T>> handlers;

		void clear() override {
			handlers.clear();
		}

		void handle(type_erased* erased_events) override {
			auto& events = static_cast<std::vector<T>&>(
					static_cast<ErasedVector<T>*>(erased_events)->data
			);
			for (auto& event: events) {
				for (auto& handler: handlers) {
					handler(event);
				}
			}
		}
	};
}

#endif //ENGINETEST_TYPE_ERASED_H
