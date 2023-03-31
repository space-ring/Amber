//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_STAGE_H
#define ENGINE_STAGE_H

#include "graphics.h"
#include <string>
#include "events.h"
#include <list>
#include <queue>
#include <map>
#include "EventManager.h"

namespace Amber {

	class Engine;

	class Scene;

	class Stage {
		using string = std::string;

	private:
		string name;
		int x, y, width, height;
		GLFWwindow* window = nullptr;
		std::map<string, Scene*>* scenes = new std::map<string, Scene*>;
		Scene* front = nullptr;
	public:

		Engine& engine;

		bool focused = false;

		Stage(Engine& engine, std::string_view name, int x, int y, int width, int height);

		virtual ~Stage();

		/* Initialises GLFW window and OpenGL context */
		void init();

		void render();

		void poll();

		void update();

		void pick();

		void show();

		void hide();

		GLFWwindow* getWindow() const;

		void addScene(const string& id, Scene* scene);

		void setFrontScene(const string& scene);

		Scene* getFront() const;

	};
}

#endif //ENGINE_STAGE_H
