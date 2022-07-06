//
// Created by Chris on 30/06/2022.
//

#ifndef ENGINE_EVENTMANAGER_H
#define ENGINE_EVENTMANAGER_H

#include <map>
#include <vector>
#include "event.h"
#include "Engine.h"

class EventManager {

    template<class Base, class Event>
    using handlerVector = std::vector<EventHandler<Base, Event>>;

    template<class Base, class Event>
    static handlerVector<Base, Event> handler; //forces singleton EventManager

public:
    // add any handler
    template<class Base, class Event>
    void addHandler(const EventHandler<Base, Event>& handler) { //handler forces when Event : Base, no check required
        EventManager::handler<Base, Event>.push_back(handler);
    }

    template<class Base, class Event>
    void clearHandlers() {
        Base* p = static_cast<Event*>(nullptr);
        EventManager::handler<Base, Event>.clear();
    }

    // call any handler
    template<class Base, class Event>
    void onEvent(const Event& event) {
        Base* p = static_cast<Event*>(nullptr);
        for (EventHandler<Base, Event>& handler: EventManager::handler<Base, Event>) {
            handler(event);
        }
    }

    template<class T, class... Args>
    static void onGLFWevent(GLFWwindow* window, Args... args) {
        auto* p = static_cast<Engine*>(glfwGetWindowUserPointer(window));
        p->events->template onEvent<window_events::Event, T>(T{window, args...}); //onEvent checks T : window::event
    }
};

template<class Base, class Event>
std::vector<EventHandler<Base, Event>> EventManager::handler;


#endif //ENGINE_EVENTMANAGER_H
