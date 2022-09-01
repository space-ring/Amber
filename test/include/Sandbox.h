//
// Created by croissant on 19/07/2022.
//

#ifndef ENGINE_SANDBOX_H
#define ENGINE_SANDBOX_H

#include "event.h"
#include "EventManager.h"

struct Sandbox {

	bool running = true;
	Amber::EventManager handlers;

	Sandbox();

	void update() {

	}
};


#endif //ENGINE_SANDBOX_H
