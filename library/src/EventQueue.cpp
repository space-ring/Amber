//
// Created by croissant on 14/09/2022.
//

#include "EventQueue.h"

void Amber::EventQueue::clearEvents() {
	for (auto& [type, list]: events) {
		delete list;
	}
	events.clear();
}
