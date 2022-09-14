//
// Created by Chris on 06/04/2022.
//

#ifndef ENGINE_EVENT_H
#define ENGINE_EVENT_H

#include "graphics.h"
#include "variadic.h"
#include  <iostream>
#include "IClonable.h"

namespace Amber {
	struct Event {
		bool handled = false;
	};

	template<class T>
	class Handler {
		typedef std::function<void(T&)> handler;

		const handler function;

	public :
		bool active = true;

		Handler(const handler& handler) : function(handler) {}

		void operator()(T& event) {
			function(event);
		}
	};

	using EventHandler = Handler<Event>;

	namespace scene_events {
		struct Event {
			bool handled = false;
		};

		using SceneEventHandler = Handler<Amber::scene_events::Event>;

	}

	namespace window_events {

		struct Event {
			bool handled = false;
			const GLFWwindow* window;
		};

		struct EnterEvent {
			EnterEvent(const GLFWwindow* window, const int entered);

			bool handled = false;
			const GLFWwindow* window;
			const int entered;
		};

		struct FocusEvent {
			FocusEvent(const GLFWwindow* window, const int focused);

			bool handled = false;
			const GLFWwindow* window;
			const int focused;
		};

		struct CloseEvent {
			explicit CloseEvent(const GLFWwindow* window);

			bool handled = false;
			const GLFWwindow* window;
		};

		struct KeyEvent {
			KeyEvent(const GLFWwindow* window, const int key, const int scancode, const int action, const int mods);

			bool handled = false;
			const GLFWwindow* window;
			const int key;
			const int scancode;
			const int action;
			const int mods;
		};

		struct CharEvent {
			CharEvent(const GLFWwindow* window, const unsigned int codepoint);

			bool handled = false;
			const GLFWwindow* window;
			const unsigned int codepoint;
		};

		struct MotionEvent {
			MotionEvent(const GLFWwindow* window, const double xpos, const double ypos);

			bool handled = false;
			const GLFWwindow* window;
			const double xpos;
			const double ypos;
		};

		struct ClickEvent {
			ClickEvent(const GLFWwindow* window, const int button, const int action, const int mods);

			bool handled = false;
			const GLFWwindow* window;
			const int button;
			const int action;
			const int mods;
		};

		struct ScrollEvent {
			ScrollEvent(const GLFWwindow* window, const double xoffset, const double yoffset);

			bool handled = false;
			const GLFWwindow* window;
			const double xoffset;
			const double yoffset;
		};

		struct FramebufferSizeEvent {
			FramebufferSizeEvent(const GLFWwindow* window, const int width, const int height);

			bool handled = false;
			const GLFWwindow* window;
			const int width;
			const int height;
		};

		//todo joystick, gamepad

		using WindowEventHandler = Handler<Amber::window_events::Event>;
		using EnterHandler = Handler<EnterEvent>;
		using FocusHandler = Handler<FocusEvent>;
		using CloseHandler = Handler<CloseEvent>;
		using KeyHandler = Handler<KeyEvent>;
		using CharHandler = Handler<CharEvent>;
		using MotionHandler = Handler<MotionEvent>;
		using ClickHandler = Handler<ClickEvent>;
		using ScrollHandler = Handler<ScrollEvent>;
		using FramebufferSizeHandler = Handler<FramebufferSizeEvent>;
	}
}
#endif //ENGINE_EVENT_H
