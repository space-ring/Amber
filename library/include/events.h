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
	struct Event { //todo list init / constructors ?
		bool handled = false;

		Event() = default;

		virtual ~Event() = default;

		virtual Event* clone() const = 0;
	};

	template<class T>
	using CloneEvent = IClonable<Event, T>;

	template<class T>
	class EventHandler {
		typedef std::function<void(Event&)> base_handler;
		typedef std::function<void(T&)> derived_handler;

		const derived_handler function;

		static const derived_handler& check(const derived_handler& h) { //todo concepts?
			Event* p = static_cast<T*>(nullptr);
			return h;
		}

	public:
		bool active = true;

		static base_handler upcast(const derived_handler& handler) { // existence of handler already checks T : Event
			return [handler](Event& event) -> void {
				handler(dynamic_cast<T&>(event)); //downcast for call, dynamic to ensure no sibling event cast
			};
		}

		EventHandler(const derived_handler& handler) :
				function(check(handler)) {} //check T : Event

		void operator()(T& event) {
			function(event);
		}
	};

	using GenericHandler = EventHandler<Event>;

	namespace scene_events {
		struct Event : Amber::Event {

		};

		struct EnterEvent : Event {

		};

		using EnterHandler = EventHandler<EnterEvent>;

	}

	namespace window_events {

		struct Event : IClonable<Amber::Event, Event> {
			const GLFWwindow* window;

			Event(const GLFWwindow* window);
		};

		struct EnterEvent : IClonable<Event, EnterEvent> {
			const int entered;

			EnterEvent(const GLFWwindow* window, const int entered);
		};

		struct FocusEvent : IClonable<Event, FocusEvent> {
			const int focused;

			FocusEvent(const GLFWwindow* window, const int focused);
		};

		struct CloseEvent : IClonable<Event, CloseEvent> {
			CloseEvent(const GLFWwindow* window);
		};

		struct KeyEvent : IClonable<Event, KeyEvent> {
			const int key;
			const int scancode;
			const int action;
			const int mods;

			KeyEvent(const GLFWwindow* window, const int key, const int scancode, const int action, const int mods);
		};

		struct CharEvent : IClonable<Event, CharEvent> {
			const unsigned int codepoint;

			CharEvent(const GLFWwindow* window, const unsigned int codepoint);
		};

		struct MotionEvent : IClonable<Event, MotionEvent> {
			const double xpos;
			const double ypos;

			MotionEvent(const GLFWwindow* window, const double xpos, const double ypos);
		};

		struct ClickEvent : IClonable<Event, ClickEvent> {
			const int button;
			const int action;
			const int mods;

			ClickEvent(const GLFWwindow* window, const int button, const int action, const int mods);
		};

		struct ScrollEvent : IClonable<Event, ScrollEvent> {
			const double xoffset;
			const double yoffset;

			ScrollEvent(const GLFWwindow* window, const double xoffset, const double yoffset);
		};

		struct FramebufferSizeEvent : IClonable<Event, FramebufferSizeEvent> {
			const int width;
			const int height;

			FramebufferSizeEvent(const GLFWwindow* window, const int width, const int height);
		};

		//todo joystick, gamepad

		using EnterHandler = EventHandler<EnterEvent>;
		using FocusHandler = EventHandler<FocusEvent>;
		using CloseHandler = EventHandler<CloseEvent>;
		using KeyHandler = EventHandler<KeyEvent>;
		using CharHandler = EventHandler<CharEvent>;
		using MotionHandler = EventHandler<MotionEvent>;
		using ClickHandler = EventHandler<ClickEvent>;
		using ScrollHandler = EventHandler<ScrollEvent>;
		using FramebufferSizeHandler = EventHandler<FramebufferSizeEvent>;
	}
}
#endif //ENGINE_EVENT_H
