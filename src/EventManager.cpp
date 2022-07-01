//
// Created by Chris on 30/06/2022.
//

#include "EventManager.h"


void EventManager::clearHandlers(long id) {
    if (handlers->contains(id)) {
        handlers->at(id)->clear();
    }
}

EventManager::~EventManager() {
    for (auto& vector: *handlers) {
        vector.second->clear();
        delete vector.second;
    }
    delete handlers;
}
