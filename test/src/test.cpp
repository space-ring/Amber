#include <iostream>
#include <thread>
#include "Application.h"
#include "snake.h"
#include "scenes/demo/SnakeScene.h"
#include "scenes/demo/DemoScene.h"

int main() {

	using namespace Amber;
	EventManager m;


	int bw = 50, bh = 50;
	int width = 500, height = 500;

	Amber::Application<SnakeGame> app("Worm", 1030, 470, width, height, std::chrono::milliseconds(50), bw, bh);

	auto& assets = app.engine.assets;
	assets.addManifest("assets/manifest");

	auto& stage = app.engine.stage;
	stage.addScene("snake", new SnakeScene(bw, bh));
	stage.setFrontScene("snake");

	app.run();

	// program close
	return 0;
}