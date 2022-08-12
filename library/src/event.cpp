//
// Created by Chris on 28/06/2022.
//

#include "event.h"

namespace Amber {
    unsigned long handler_counter = 0;

    Event::Event() : handled(false) {};

    window_events::Event::Event(const GLFWwindow* window) : window(window) {}

    window_events::EnterEvent::EnterEvent(const GLFWwindow* window, const int entered) : Event(window),
                                                                                         entered(entered) {}

    window_events::FocusEvent::FocusEvent(const GLFWwindow* window, const int focused) : Event(window),
                                                                                         focused(focused) {}

    window_events::CloseEvent::CloseEvent(const GLFWwindow* window) : Event(window) {}

    window_events::KeyEvent::KeyEvent(const GLFWwindow* window, const int key, const int scancode, const int action,
                                      const int mods) : Event(window), key(key), scancode(scancode), action(action),
                                                        mods(mods) {}

    window_events::CharEvent::CharEvent(const GLFWwindow* window, const unsigned int codepoint) : Event(window),
                                                                                                  codepoint(
                                                                                                          codepoint) {}

    window_events::MotionEvent::MotionEvent(const GLFWwindow* window, const double xpos, const double ypos) : Event(
            window),
                                                                                                              xpos(xpos),
                                                                                                              ypos(ypos) {}

    window_events::ClickEvent::ClickEvent(const GLFWwindow* window, const int button, const int action, const int mods)
            : Event(window), button(button), action(action), mods(mods) {}

    window_events::ScrollEvent::ScrollEvent(const GLFWwindow* window, const double xoffset, const double yoffset)
            : Event(
            window), xoffset(xoffset), yoffset(yoffset) {}

    Event::~Event() {

    }

    window_events::FramebufferSizeEvent::FramebufferSizeEvent(const GLFWwindow* window, const int width,const int height)
                                                              : Event(window), width(width),
                                                                                  height(height) {}
}