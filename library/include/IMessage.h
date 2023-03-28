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
#include <set>

namespace Amber {
	class IMessage {
		using eventMap = std::map<std::type_index, ErasedContainer*>;

	protected:
		eventMap events;
		std::mutex mutex;

		template<class T>
		std::vector<T>& getEvents() {
			if (!events.contains(typeid(T))) {
				events.emplace(typeid(T), new Vector<T>);
				std::cout << "created " << typeid(T).name() << " queue" << std::endl;
			}
			return static_cast<Vector<T>*>(events.at(typeid(T)))->data;
		}

	public:

		virtual ~IMessage() {
			for (auto& s: events) {
				delete s.second;
			}
		}

		template<class T>
		void putEvent(const T& event) {
			std::lock_guard lock(mutex);
			getEvents<T>().push_back(event);
		}

		void clearEvents();
	};
}

#endif //ENGINETEST_IMESSAGE_H
