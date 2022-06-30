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

        Event(const GLFWwindow* window);

        virtual ~Event() = default;
    };

    struct EnterEvent : Event {
        const int entered;

        EnterEvent(const GLFWwindow* window, int entered);
    };

    struct FocusEvent : Event {
        const int focused;

        FocusEvent(const GLFWwindow* window, int focused);
    };

    struct CloseEvent : Event {

        explicit CloseEvent(const GLFWwindow* window);
    };

    struct KeyEvent : Event {
        const int key;
        const int scancode;
        const int action;
        const int mods;

        KeyEvent(const GLFWwindow* window, int key, int scancode, int action, int mods);
    };

    struct CharEvent : Event {
        const unsigned int codepoint;

        CharEvent(const GLFWwindow* window, unsigned int codepoint);
    };

    struct MotionEvent : Event {
        const double xpos;
        const double ypos;

        MotionEvent(const GLFWwindow* window, double xpos, double ypos);
    };

    struct ClickEvent : Event {
        const int button;
        const int action;
        const int mods;

        ClickEvent(const GLFWwindow* window, int button, int action, int mods);

    };

    struct ScrollEvent : Event {
        const double xoffset;
        const double yoffset;

        ScrollEvent(const GLFWwindow* window, double xoffset, double yoffset);
    };

    //todo joystick, gamepad

    //handlers
    template<class T>
    class EventHandler : public Variadic<void, const Event&> {
        typedef std::function<void(const Event&)> base_handler;
        typedef std::function<void(const T&)> derived_handler;

    public:
        static const long type;

        static base_handler upcast(const derived_handler& handler, Event* p = nullptr) {
            return [handler](const Event& event) -> void {
                handler(static_cast<const T&>(event)); //downcast for call
            };
        }

        EventHandler(const derived_handler& handler) :
                Variadic<void, const Event&>(
                        upcast(handler, static_cast<T*>(nullptr)) //check T : Event
                ) {
        }

        void operator()(const Event& event) override {
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
