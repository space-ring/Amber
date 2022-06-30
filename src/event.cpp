//
// Created by Chris on 28/06/2022.
//

#include "event.h"

namespace window_event {
    long handler_counter = 0;

    Event::Event(const GLFWwindow* window) : window(window) {}

    EnterEvent::EnterEvent(const GLFWwindow* window, const int entered) : Event(window), entered(entered) {}

    FocusEvent::FocusEvent(const GLFWwindow* window, const int focused) : Event(window), focused(focused) {}

    CloseEvent::CloseEvent(const GLFWwindow* window) : Event(window) {}

    KeyEvent::KeyEvent(const GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
            : Event(window), key(key), scancode(scancode), action(action), mods(mods) {}

    CharEvent::CharEvent(const GLFWwindow* window, const unsigned int codepoint) : Event(window),
                                                                                   codepoint(codepoint) {}

    MotionEvent::MotionEvent(const GLFWwindow* window, const double xpos, const double ypos) : Event(window),
                                                                                               xpos(xpos), ypos(ypos) {}

    ClickEvent::ClickEvent(const GLFWwindow* window, const int button, const int action, const int mods) : Event(
            window), button(button), action(action), mods(mods) {}

    ScrollEvent::ScrollEvent(const GLFWwindow* window, const double xoffset, const double yoffset) : Event(window),
                                                                                                     xoffset(xoffset),
                                                                                                     yoffset(yoffset) {}
}