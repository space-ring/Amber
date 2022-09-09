#include <iostream>
#include <thread>
#include "Application.h"
#include "AssetManager.h"
#include "snake.h"
#include "scenes/demo/SnakeScene.h"
#include "scenes/demo/DemoScene.h"

int main() {

	std::cout << "main thread " << std::this_thread::get_id() << std::endl;

	int width = 300, height = 300;

	Amber::Application<SnakeGame> app("Snake", 1030, 470, width, height, width, height);

	auto& assets = Amber::AssetManager::getInstance();
	assets.addManifest("assets/manifest");

	SnakeScene::getInstance(width, height);

	auto& stage = Amber::Stage::getInstance();
	stage.addScene("snake", &SnakeScene::getInstance());
	stage.setFrontScene("snake");

	app.run();

	// program close
	return 0;
}