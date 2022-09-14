//
// Created by croissant on 25/02/2022.
//

#include "Stage.h"
#include "graphics.h"
#include "EventManager.h"
#include <iostream>
#include <ctime>
#include "Engine.h"

namespace Amber {

	template<class T, class... Args>
	static void onGLFWevent(GLFWwindow* window, Args... args) {
		auto* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
		T event{window, args...};
		if (engine->stage.getFront())
			engine->stage.getFront()->onEvent<T>(event);

		if (!event.handled)
			engine->handlers.onEvent(event);
	}

	Stage::Stage(Engine& engine, const string& name, int x, int y, int width, int height)
			: engine(engine), name(name), x(x), y(y), width(width), height(height) {}

	Stage::~Stage() {
		//todo remove delete scene here. ownership.
		delete scenes;
		glfwTerminate();
	}

	void Stage::init() {
		std::cout << "GL on thread " << std::this_thread::get_id() << std::endl;
		// set up GL context
		if (!glfwInit()) {
			exit(1);
		}

		glfwWindowHint(GLFW_VISIBLE, 0);

		window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
		if (!window) {
			glfwTerminate();
			exit(2);
		}

		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
			exit(3);
		}

		glfwSetWindowPos(window, x, y);
		glfwSwapInterval(1);
		printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

		int pixWidth, pixHeight;
		glfwGetFramebufferSize(window, &pixWidth, &pixHeight);
		glViewport(0, 0, pixWidth, pixHeight);

		glfwSetWindowUserPointer(window, &engine);

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

		if (front) front->build();
	}

	void Stage::render() {
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (front) {
			front->render();
		}
		glfwSwapBuffers(window);
	}

	void Stage::update() {
		if (front) {
			front->update();
		}
	}

	void Stage::poll() {
		if (focused) glfwPollEvents();
		else glfwWaitEvents();
	}

	void Stage::pick() {
		int ww, wh;
		glfwGetWindowSize(window, &ww, &wh);
		double dx, dy;
		glfwGetCursorPos(window, &dx, &dy);
		int mx = dx, my = dy;
		if (0 <= mx && mx < ww && 0 <= my && my < wh)
			if (front) {
				//save old clear values
				float colour[4];
				double depth;
				int stencil;
				glGetFloatv(GL_COLOR_CLEAR_VALUE, &colour[0]);
				glGetDoublev(GL_DEPTH_CLEAR_VALUE, &depth);
				glGetIntegerv(GL_STENCIL_CLEAR_VALUE, &stencil);
				front->pick(mx, height - my);
				//restore old clear values
				glClearColor(colour[0], colour[1], colour[2], colour[3]);
				glClearDepth(depth);
				glClearStencil(stencil);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}
	}

	void Stage::show() {
		glfwShowWindow(window);
	}

	void Stage::hide() {
		glfwHideWindow(window);
	}

	GLFWwindow* Stage::getWindow() const {
		return window;
	}

	void Stage::addScene(const string& id, Scene* scene) {
		scenes->insert(std::pair(id, scene));
		scene->stage = this;
	}

	void Stage::setFrontScene(const string& scene) {
		if (scenes->contains(scene)) {
			if (front) front->hide();
			front = scenes->at(scene);
			if (window) front->build();
			front->show();
		}
	}

	Scene* Stage::getFront() const {
		return front;
	}

}