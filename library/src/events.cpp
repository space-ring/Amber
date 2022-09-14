//
// Created by Chris on 28/06/2022.
//

#include "events.h"

namespace Amber {
	window_events::MotionEvent::MotionEvent(const GLFWwindow* window, const double xpos, const double ypos)
			: window(window), xpos(xpos), ypos(ypos) {}

	window_events::FocusEvent::FocusEvent(const GLFWwindow* window, const int focused) : window(window),
	                                                                                            focused(focused) {}

	window_events::FramebufferSizeEvent::FramebufferSizeEvent(const GLFWwindow* window, const int width,
	                                                                 const int height) : window(window), width(width),
	                                                                                     height(height) {}

	window_events::ScrollEvent::ScrollEvent(const GLFWwindow* window, const double xoffset, const double yoffset)
			: window(window), xoffset(xoffset), yoffset(yoffset) {}

	window_events::CloseEvent::CloseEvent(const GLFWwindow* window) : window(window) {}

	window_events::KeyEvent::KeyEvent(const GLFWwindow* window, const int key, const int scancode,
	                                         const int action, const int mods) : window(window), key(key),
	                                                                             scancode(scancode), action(action),
	                                                                             mods(mods) {}

	window_events::CharEvent::CharEvent(const GLFWwindow* window, const unsigned int codepoint) : window(window),
	                                                                                                     codepoint(
			                                                                                                     codepoint) {}

	window_events::EnterEvent::EnterEvent(const GLFWwindow* window, const int entered) : window(window),
	                                                                                            entered(entered) {}

	window_events::ClickEvent::ClickEvent(const GLFWwindow* window, const int button, const int action,
	                                             const int mods) : window(window), button(button), action(action),
	                                                               mods(mods) {}
}
