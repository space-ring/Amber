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
#include <thread>
#include "EventManager.h"
#include "EventQueue.h"
#include "AssetManager.h"

namespace Amber {

	class Scene;

	class Stage final {

		std::string name;
		int x, y, width, height;
		GLFWwindow* window = nullptr;

		bool focused = false;
		bool running = true;

	public:

		EventQueue& eventOutStream;
		EventManager handlers;
		AssetManager assets;
		Scene& front;

		Stage(Scene& front, EventQueue& stream, std::string_view manifest, std::string_view name, int x, int y, int width, int height);

		~Stage();

		Stage(const Stage&) = delete;

		Stage(Stage&&) = delete;

		Stage& operator=(const Stage&) = delete;

		Stage& operator=(Stage&&) = delete;

		void render();

		void poll();

		void update();

		void pick();

		void show();

		void hide();

		GLFWwindow* getWindow() const;

		bool isRunning() const;

		void kill();
	};
}

#endif //ENGINE_STAGE_H
