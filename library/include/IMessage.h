//
// Created by croissant on 09/09/2022.
//

#ifndef ENGINETEST_IMESSAGE_H
#define ENGINETEST_IMESSAGE_H

#include <mutex>
#include <deque>
#include "events.h"
#include "type_erased.h"
#include <map>
#include <typeindex>

namespace Amber {
	class IMessage {
		using eventTypes = std::set<std::type_index>;
		using eventMap = std::map<std::type_index, type_erased*>;

	protected:
		eventTypes types;
		eventMap events;
		std::mutex mutex;

	public:

		virtual ~IMessage() {
			for (auto s: events) {
				delete s.second;
			}
		}

		template<class T>
		std::vector<T>& getEvents() {
			if (!events.contains(typeid(T)))
				events.emplace(typeid(T), new ErasedVector<T>);
			return static_cast<ErasedVector<T>*>(events.at(typeid(T)))->data;
		}

		template<class T>
		void putEvent(const T& event) {
			std::lock_guard lock(mutex);
			getEvents<T>().push_back(T(event));
		}

		const eventTypes& getTypes() const {
			return types;
		}
	};
}

#endif //ENGINETEST_IMESSAGE_H
