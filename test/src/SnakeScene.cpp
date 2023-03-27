//
// Created by croissant on 01/09/2022.
//

#include "scenes/demo/SnakeScene.h"
#include "StateBuffer.h"
#include "snake.h"
#include "Application.h"

SnakeScene::SnakeScene(unsigned int width, unsigned int height)
		: camera(glm::vec3(0), glm::vec3(0), 70, width, height, 0, 100, 0, 100) {
	handlers.addHandler(Amber::window_events::FramebufferSizeHandler(
			[&](Amber::window_events::FramebufferSizeEvent& e) {
				camera.setAspectRatio((float) e.width, (float) e.height);
			}));

	handlers.addHandler(Amber::window_events::KeyHandler(
			[&](Amber::window_events::KeyEvent& e) {
				if (e.action == 1) {
					auto& app = stage->engine.application;
					switch (e.key) {
						case GLFW_KEY_S:
							app.putEvent(SnakeEvents::DirEvent(DOWN));
							break;

						case GLFW_KEY_W:
							app.putEvent(SnakeEvents::DirEvent(UP));
							break;

						case GLFW_KEY_A:
							app.putEvent(SnakeEvents::DirEvent(LEFT));
							break;

						case GLFW_KEY_D:
							app.putEvent(SnakeEvents::DirEvent(RIGHT));
							break;
					}
				}
			}
	));

}

SnakeScene::~SnakeScene() {

}

void SnakeScene::build() {
	auto& app = static_cast<Application<SnakeGame>&>(stage->engine.application);
	models.addMesh(&stage->engine.assets.getMesh(0), app.R.height * app.R.height+2);
}

void SnakeScene::show() {

}

void SnakeScene::hide() {

}

void SnakeScene::update() {
	//send key events
	auto& app = static_cast<Application<SnakeGame>&>(stage->engine.application);

	SnakeGame::R& game = app.R;
	segments.clear();

	Amber::Mesh* plane = &stage->engine.assets.getMesh(0);

	for (auto& p: game.snake.segments) {
		auto& segment = segments.emplace_back();
		segment.setMesh(plane);
		segment.transform.translate(glm::vec3(p.x, p.y, -1));
		segment.transform.scale(glm::vec3(0.5));
		models.addModel(segment);
	}

	auto& fruit = segments.emplace_back();
	fruit.setMesh(plane);
	fruit.transform.scale(glm::vec3(0.5));
	fruit.transform.translate(glm::vec3(game.fruit.x, game.fruit.y, -1));
	models.addModel(fruit);

	models.buffer(plane);
}

void SnakeScene::pick(int x, int y) {

}

void SnakeScene::render() {
	auto& assets = stage->engine.assets;

	Amber::Mesh* plane = &assets.getMesh(0);
	Amber::Shader* shader = &assets.getShader(0).start();

	glBindVertexArray(plane->getVao());
	glUniformMatrix4fv(14, 1, false, glm::value_ptr(camera.getView()));
	glUniformMatrix4fv(18, 1, false, glm::value_ptr(camera.getOrthogonal()));
	models.link(plane);
	glDrawElementsInstanced(GL_TRIANGLES,
							plane->elementCount,
							GL_UNSIGNED_INT, 0,
							models.getRenderCount(plane));
	glBindVertexArray(0);
	shader->stop();

}
