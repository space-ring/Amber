//
// Created by croissant on 14/09/2022.
//

#include "IMessage.h"

void Amber::IMessage::clearEvents(std::type_index type) {
	if (events.contains(type)) events.at(type)->clear();
}
