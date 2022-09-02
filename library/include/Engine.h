//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include <map>
#include "Scene.h"
#include "graphics.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include <thread>
#include <mutex>
#include <queue>
#include "StateBuffer.h"
#include "EventManager.h"
#include "AssetManager.h"
#include "NoDefaultSingleton.h"

//todo default members for resources (VAO = 0, etc)
//todo initialise members that don't depend on constructor in class declaration ^^^
//todo use references instead of pointers
//todo circular dependencies with IO
//todo make everything public?
//todo move semantics (engine should own almost everything)
//todo destructor calls destroy (so can terminate graphics manually)
//todo some vectors need to be changed to lists when random access not needed
//todo move away from string references
//todo resource classes should adhere to the big 5.
namespace Amber {
	class Engine : public NoDefaultSingleton<Engine> {
		using string = std::string;

		friend Singleton<Engine>;

	private:
		volatile bool running = false;

		Engine(const string& name, int x, int y, int width, int height);

		virtual ~Engine();

	public:
		EventManager handlers;

		/* Initialises all listed resources, scenes and stages. Enters main renderState loop */
		void init();

		/* Requests for the window to shut down */
		void kill();

		bool getRunning() const;

	};
}

#endif //ENGINE_ENGINE_H
