#include <iostream>
#include "Application.h"
#include "snake.h"
#include "scenes/demo/SnakeScene.h"

//int main() {
//	using namespace Amber;
//	int bw = 50, bh = 50;
//	int width = 500, height = 500;
//
//	Amber::Application<SnakeGame> app("Worm", 1030, 470, width, height, bw, bh);
//
//	auto& assets = app.engine.assets;
//	assets.addManifest("assets/manifest");
//
//	auto& stage = app.engine.stage;
//	stage.addScene("snake", new SnakeScene(bw, bh));
//	stage.setFrontScene("snake");
//
//	app.run(std::chrono::milliseconds(50));
//
//	return 0;
//}

class SnakeApp : public Application<SnakeGame> {

	SnakeScene scene{stage, 500, 500};

public:
	template<class... Args>
	SnakeApp(std::string_view manifest, std::string_view name, int x, int y, int width, int height, Args... args)
			: Application(scene, manifest, name, x, y, width, height, args...) {

		stage.handlers.addHandler(window_events::CloseHandler([&](window_events::CloseEvent& e) {
			stage.kill();
		}));
	}
};

int main() {

	SnakeApp app("assets/manifest", "Worm", 1030, 470, 500, 500, 50, 50);
	app.run(std::chrono::milliseconds(50));

}