//
// Created by croissant on 09/09/2022.
//

#ifndef ENGINETEST_IMESSAGE_H
#define ENGINETEST_IMESSAGE_H

#include <mutex>
#include <deque>
#include "events.h"

namespace Amber {
	class IMessage {
	protected:
		std::deque<Event*> events;
		std::mutex mutex;

	public:
		void putEvent(const Event& event) {
			std::lock_guard lock(mutex);
			events.push_back(event.clone());
		}
	};
}

#endif //ENGINETEST_IMESSAGE_H
