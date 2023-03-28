//
// Created by croissant on 14/09/2022.
//

#include "IMessage.h"

void Amber::IMessage::clearEvents() {
	for (auto& [type, list]: events) {
		delete list;
	}
	events.clear();
}
