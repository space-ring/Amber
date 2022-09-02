#include <iostream>
#include <thread>
#include "Engine.h"
#include "scenes/demo/DemoScene.h"
#include "ThreadedEngine.h"
#include "AssetManager.h"
#include "snake.h"

int main() {

	std::cout << "main thread " << std::this_thread::get_id() << std::endl;

	// 1030,470

	Amber::ThreadedEngine<SnakeGame> app(SnakeGame{100, 100}, "Snake", 1030, 470, 500, 500);
	Amber::Engine& engine = Amber::Engine::getInstance();
	auto& game = app.game;
	auto& assets = Amber::AssetManager::getInstance();
	auto& stage = Amber::Stage::getInstance();

	assets.addManifest("assets/manifest");
	stage.addScene("demo", &DemoScene::getInstance());
	stage.setFrontScene("demo");

	app.run();

	// program close
	return 0;
}