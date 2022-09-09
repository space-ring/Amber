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

}

SnakeScene::~SnakeScene() {

}

void SnakeScene::build() {
	Amber::AssetManager::getInstance().buildAll();

	Model& m = models.newModel();
	m.translate(glm::vec3(0, 0, -1));
	m.scale(glm::vec3(5, 5, 1));
	m.setMesh(Amber::AssetManager::getInstance().getMesh("plane")->build());
//	models.add(m);
}

void SnakeScene::show() {

}

void SnakeScene::hide() {

}

void SnakeScene::update() {
	//send key events

	if (keys.down.contains(GLFW_KEY_S)){
		auto& a = Application<SnakeGame>::getInstance();
		a.putEvent(SnakeEvents::DirEvent{DOWN});
	}

	SnakeGame::R& game = StateBuffer<SnakeGame>::getInstance().getRenderState();
	for (auto& seg: segments) {
		models.remove(seg);
	}
	segments.clear();

	Amber::Mesh* plane = Amber::AssetManager::getInstance().getMesh("plane")->build();

	for (auto& p: game.snake.segments) {
		std::cout << std::endl;
		std::cout << p.x << " " << p.y << std::endl;
		segments.emplace_back(plane);
		segments.back().translate(glm::vec3(p.x, p.y, -1));
		models.add(segments.back(), camera.width * camera.height);
	}

	models.buffer(plane);
}

void SnakeScene::pick(int x, int y) {

}

void SnakeScene::render() {

	Amber::Mesh* plane = Amber::AssetManager::getInstance().getMesh("plane")->build();
	Amber::Shader* shader = Amber::AssetManager::getInstance().getShader("basic")->start();

	glBindVertexArray(plane->getVao());
	glUniformMatrix4fv(14, 1, false, glm::value_ptr(camera.getView()));
	glUniformMatrix4fv(18, 1, false, glm::value_ptr(camera.getOrthogonal()));
	glDrawElementsInstanced(GL_TRIANGLES, plane->getElementCount(), GL_UNSIGNED_INT, 0, models.getRenderCount(plane));
	glBindVertexArray(0);
	shader->stop();

}
