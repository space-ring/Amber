#include <iostream>
#include <thread>
#include "Engine.h"
#include "Sandbox.h"
#include "scenes/demo/DemoScene.h"
#include "ThreadedEngine.h"
#include "AssetManager.h"

int main() {

	std::cout << "main thread " << std::this_thread::get_id() << std::endl;

	// 1030,470

	Amber::ThreadedEngine<Sandbox> app("Test game", 1030, 470, 500, 500);
	Amber::Engine& engine = Amber::Engine::getInstance();
	Sandbox& game = app.game;
	auto& assets = Amber::AssetManager::getInstance();
	auto& stage = Amber::Stage::getInstance();

	assets.addManifest("assets/manifest");
	stage.addScene("demo", &DemoScene::getInstance());
	stage.setFrontScene("demo");

	app.run();

	// program close
	return 0;
}