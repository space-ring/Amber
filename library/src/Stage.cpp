//
// Created by croissant on 25/02/2022.
//

#include "Stage.h"
#include "Scene.h"
#include "graphics.h"
#include "EventManager.h"
#include <iostream>

namespace Amber {

	template<class T, class... Args>
	static void onGLFWevent(GLFWwindow* window, Args... args) {
		auto* stage = static_cast<Stage*>(glfwGetWindowUserPointer(window));
		T event{window, args...};
		stage->front.onEvent<T>(event);

		if (!event.handled)
			stage->handlers.onEvent(event);
	}

	Stage::Stage(Scene& front, EventQueue& stream, std::string_view manifest, std::string_view name, int x, int y,
				 int width, int height)
			: front(front), assets(manifest), eventOutStream(stream), name(name), x(x), y(y), width(width),
			  height(height) {

		std::cout << "GL on thread " << std::this_thread::get_id() << std::endl;
		// set up GL context
		if (!glfwInit()) {
			throw std::runtime_error("Failed to create a GL context");
		}

		glfwWindowHint(GLFW_VISIBLE, 0);

		window = glfwCreateWindow(width, height, std::string{name}.c_str(), nullptr, nullptr);
		if (!window) {
			glfwTerminate();
			throw std::runtime_error("Failed to create a GLFW window");
		}

		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
			throw std::runtime_error("Failed to load GLAD");
		}

		glfwSetWindowPos(window, x, y);
		glfwSwapInterval(1);
		printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

		int pixWidth, pixHeight;
		glfwGetFramebufferSize(window, &pixWidth, &pixHeight);

		glfwSetWindowUserPointer(window, this);

		//default window handlers

		handlers.addHandler(window_events::CloseHandler([&](window_events::CloseEvent&) {
								hide();
								running = false;
							})
		);
		handlers.addHandler(window_events::FocusHandler([&](window_events::FocusEvent& e) {
								Stage::focused = e.focused;
							})
		);

		handlers.addHandler(window_events::KeyHandler([&](window_events::KeyEvent& e) {
								switch (e.action) {
									case GLFW_PRESS:
										keys.pressed[e.key] = true;
										keys.last[e.key] = true;
										break;
									case GLFW_RELEASE:
										keys.released[e.key] = true;
										keys.last[e.key] = false;
										break;
								}
							})
		);


		//game_events
		glfwSetCursorEnterCallback(window, onGLFWevent<window_events::EnterEvent, int>);
		glfwSetWindowCloseCallback(window, onGLFWevent<window_events::CloseEvent>);
		glfwSetWindowFocusCallback(window, onGLFWevent<window_events::FocusEvent, int>);
		glfwSetKeyCallback(window, onGLFWevent<window_events::KeyEvent, int, int, int, int>);
		glfwSetCharCallback(window, onGLFWevent<window_events::CharEvent, unsigned int>);
		glfwSetMouseButtonCallback(window, onGLFWevent<window_events::ClickEvent, int, int, int>);
		glfwSetScrollCallback(window, onGLFWevent<window_events::ScrollEvent, double, double>);
		glfwSetCursorPosCallback(window, onGLFWevent<window_events::MotionEvent, double, double>);
		glfwSetFramebufferSizeCallback(window, onGLFWevent<window_events::FramebufferSizeEvent, int, int>);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	Stage::~Stage() {
		glfwTerminate();
	}

	void Stage::render() {
		front.render();
	}

	void Stage::update() {
		keys.down = (keys.pressed & keys.released & keys.last)
					| (keys.down & ~keys.pressed & ~keys.released)
					| (~keys.down & keys.pressed & ~keys.released);

		/*
		D	P	R	L
		1	1	1	1	->	1
		1	1	1	0	->	0
		1	0	1	1	->	0
		1	0	1	0	->	0
		1	0	0	1	->	1
		1	0	0	0	->	1
		0	1	1	1	->	1
		0	1	0	1	->	1
		0	1	1	0	->	0
		0	1	0	0	->	1
		0	0	0	1	->	0
		0	0	0	0	->	0
		 */
		front.update();
	}

	void Stage::poll() {
		keys.pressed.reset();
		keys.released.reset();

		if (focused) glfwPollEvents();
		else glfwWaitEvents();
	}

	void Stage::pick() {
		//todo
//		int ww, wh;
//		glfwGetWindowSize(window, &ww, &wh);
//		double dx, dy;
//		glfwGetCursorPos(window, &dx, &dy);
//		int mx = dx, my = dy;
//		if (0 <= mx && mx < ww && 0 <= my && my < wh) {
//			//save old clear values
//			float colour[4];
//			double depth;
//			int stencil;
//			glGetFloatv(GL_COLOR_CLEAR_VALUE, &colour[0]);
//			glGetDoublev(GL_DEPTH_CLEAR_VALUE, &depth);
//			glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &stencil);
//			front.pick(mx, height - my);
//			//restore old clear values
//			glClearColor(colour[0], colour[1], colour[2], colour[3]);
//			glClearDepth(depth);
//			glClearStencil(stencil);
//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//		}
	}

	void Stage::show() {
		glfwShowWindow(window);
		focused = true;
	}

	void Stage::hide() {
		glfwHideWindow(window);
		focused = false;
	}

	GLFWwindow* Stage::getWindow() const {
		return window;
	}

	bool Stage::isRunning() const {
		return running;
	}

	void Stage::kill() {
		running = false;
	}
}