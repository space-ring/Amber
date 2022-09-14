#include <iostream>
#include <thread>
#include "Application.h"
#include "snake.h"
#include "scenes/demo/SnakeScene.h"
#include "scenes/demo/DemoScene.h"

struct C {
	virtual ~C() {

	}
};

struct A : C {};

int main() {

	A a;
	C& c = a;

	decltype(c) p = static_cast<decltype(c)>(c);

	std::cout << typeid(p).name() << std::endl;

	Amber::EventManager manager;
	manager.getHandlers<window_events::KeyEvent>();

	auto handler = Amber::Handler<Amber::window_events::KeyEvent>(
			[](window_events::KeyEvent& event) {}
	);

	EventManager d;
	d.addHandler(
			Handler<window_events::EnterEvent>
					([](Amber::window_events::EnterEvent&) {}));


	int bw = 50, bh = 50;
	int width = 500, height = 500;

	Amber::Application<SnakeGame> app("Worm", 1030, 470, width, height, bw, bh);

	auto& assets = app.engine.assets;
	assets.addManifest("assets/manifest");

	auto& stage = app.engine.stage;
	stage.addScene("snake", new SnakeScene(bw, bh));
	stage.setFrontScene("snake");

	app.run();

	// program close
	return 0;
}