//
// Created by Chris on 06/04/2022.
//

#ifndef ENGINE_EVENT_H
#define ENGINE_EVENT_H

#include "graphics.h"
#include  <iostream>
#include <functional>

template<class BaseEvent, class DerivedEvent>
class EventHandler {
    typedef std::function<void(const DerivedEvent&)> derived_handler;

    const derived_handler function;

    static const derived_handler& check(const derived_handler& h) {
        BaseEvent* p = static_cast<DerivedEvent*>(nullptr); //check DerivedEvent : BaseEvent
        return h;
    }

public:
    bool active{true};

    EventHandler(const derived_handler& handler) :
            function(
                    check(handler) //check DerivedEvent : BaseEvent
            ) {
    }

    void operator()(const DerivedEvent& event) {
        function(event);
    }
};

namespace scene_events { //todo scene events
    struct Event {

    };

    struct EnterEvent : Event {

    };

    template<class T>
    using EventHandler = EventHandler<Event, T>;
    using EnterHandler = EventHandler<EnterEvent>;

}

namespace window_events {

    enum class event_type {
        CLOSE,
        ENTER,
        FOCUS,
        KEY,
        CHAR,
        CLICK,
        SCROLL,
        MOTION
    };

    struct Event { //todo remove list init because it allows incomplete init
        const GLFWwindow* window;
    };

    struct EnterEvent : Event {
        const int entered;
    };

    struct FocusEvent : Event {
        const int focused;
    };

    struct CloseEvent : Event {
    };

    struct KeyEvent : Event {
        const int key;
        const int scancode;
        const int action;
        const int mods;
    };

    struct CharEvent : Event {
        const unsigned int codepoint;
    };

    struct MotionEvent : Event {
        const double xpos;
        const double ypos;
    };

    struct ClickEvent : Event {
        const int button;
        const int action;
        const int mods;
    };

    struct ScrollEvent : Event {
        const double xoffset;
        const double yoffset;
    };

    //todo joystick, gamepad

    template<class T>
    using EventHandler = EventHandler<Event, T>;

    using GenericHandler = EventHandler<Event>;
    using EnterHandler = EventHandler<EnterEvent>;
    using FocusHandler = EventHandler<FocusEvent>;
    using CloseHandler = EventHandler<CloseEvent>;
    using KeyHandler = EventHandler<KeyEvent>;
    using CharHandler = EventHandler<CharEvent>;
    using MotionHandler = EventHandler<MotionEvent>;
    using ClickHandler = EventHandler<ClickEvent>;
    using ScrollHandler = EventHandler<ScrollEvent>;
}

#endif //ENGINE_EVENT_H
