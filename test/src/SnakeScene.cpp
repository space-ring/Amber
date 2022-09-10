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
	auto& assets = stage->engine.assets;
	assets.buildAll();
	Model& m = models.newModel();
	m.translate(glm::vec3(0, 0, -1));
	m.scale(glm::vec3(5, 5, 1));
	m.setMesh(assets.getMesh("plane")->build());
//	models.add(m);
}

void SnakeScene::show() {

}

void SnakeScene::hide() {

}

void SnakeScene::update() {
	//send key events
	auto& app = static_cast<Application<SnakeGame>&>(stage->engine.application);

	SnakeGame::R& game = app.buffer.getRenderState();
	segments.clear();

	Amber::Mesh* plane = stage->engine.assets.getMesh("plane")->build();

	for (auto& p: game.snake.segments) {
		auto& segment = segments.emplace_back(plane);
		segment.translate(glm::vec3(p.x, p.y, -1));
		segment.scale(glm::vec3(0.5));
		models.add(segment, camera.width * camera.height + 1);
	}

	auto& fruit = segments.emplace_back(plane);
	fruit.scale(glm::vec3(0.5));
	fruit.translate(glm::vec3(game.fruit.x, game.fruit.y, -1));
	models.add(fruit);

	models.buffer(plane);
}

void SnakeScene::pick(int x, int y) {

}

void SnakeScene::render() {
	auto& assets = stage->engine.assets;

	Amber::Mesh* plane = assets.getMesh("plane")->build();
	Amber::Shader* shader = assets.getShader("basic")->start();

	glBindVertexArray(plane->getVao());
	glUniformMatrix4fv(14, 1, false, glm::value_ptr(camera.getView()));
	glUniformMatrix4fv(18, 1, false, glm::value_ptr(camera.getOrthogonal()));
	glDrawElementsInstanced(GL_TRIANGLES, plane->getElementCount(), GL_UNSIGNED_INT, 0, models.getRenderCount(plane));
	glBindVertexArray(0);
	shader->stop();

}
