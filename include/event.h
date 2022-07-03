//
// Created by Chris on 06/04/2022.
//

#ifndef ENGINE_EVENT_H
#define ENGINE_EVENT_H

#include "graphics.h"
#include "variadic.h"
#include  <iostream>

namespace scene_event {


}

namespace window_event {

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

    struct Event {
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

    //handlers
    template<class T>
    class EventHandler {
        typedef std::function<void(const Event&)> base_handler;
        typedef std::function<void(const T&)> derived_handler;

        const derived_handler function;

        static const derived_handler& check(const derived_handler& h) {
            Event* p = static_cast<T*>(nullptr);
            return h;
        }

    public:
        static const long type;
        bool active{true};

        static base_handler upcast(const derived_handler& handler) { // existence of handler already checks T : Event
            return [handler](const Event& event) -> void {
                handler(static_cast<const T&>(event)); //downcast for call, static since inh checked already
            };
        }

        EventHandler(const derived_handler& handler) :
                function(
                        check(handler) //check T : Event
                ) {
        }

        void operator()(const T& event) {
            function(event); //todo upcast if commenting this function throws error (return void)
        }
    };

    extern long handler_counter;
    template<class T>
    const long EventHandler<T>::type = handler_counter++;

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
