//
// Created by croissant on 01/09/2022.
//

#include "scenes/demo/SnakeScene.h"
#include "snake.h"
#include "Application.h"
#include "Texture.h"
#include "manifest.h"

SnakeScene::SnakeScene(Stage& stage, unsigned int imWidth, unsigned int imHeight)
		: Scene(stage), camera(glm::vec3(0), glm::vec3(0), 70, imWidth, imHeight, 0, 100, 0, 100) {

	handlers.addHandler(Amber::window_events::KeyHandler(
			[&](Amber::window_events::KeyEvent& e) {
				auto& eventQueue = stage.eventOutStream;
				if (e.action == 1) {
					switch (e.key) {
						case GLFW_KEY_S:
							eventQueue.putEvent(SnakeEvents::DirEvent(DOWN));
							break;

						case GLFW_KEY_W:
							eventQueue.putEvent(SnakeEvents::DirEvent(UP));
							break;

						case GLFW_KEY_A:
							eventQueue.putEvent(SnakeEvents::DirEvent(LEFT));
							break;

						case GLFW_KEY_D:
							eventQueue.putEvent(SnakeEvents::DirEvent(RIGHT));
							break;
					}
				}

				if (e.key == GLFW_KEY_ESCAPE) {
					stage.handlers.onEvent(window_events::CloseEvent{stage.getWindow()});
				}
			}
	));

	auto& plane = stage.assets.getMesh(SnakeAssets::PLANE);
	models.addMesh(&plane, 50 * 50 + 2);

	auto& fruit = models.newModel();
	fruit.setMesh(&plane);
	fruit.transform.scale(glm::vec3(5));
	models.addModel(fruit);

	handlers.addHandler(Amber::Handler<SnakeEvents::Spawn>(
			[&](SnakeEvents::Spawn& e) {
				auto& segment = segments.emplace_front();
				segment.setMesh(&plane);
				segment.transform.translate(glm::vec3(e.x * 10, e.y * 10, -1));
				segment.transform.scale(glm::vec3(5));
				models.addModel(segment);
				colours.push_back((float) 0xff0000 / 0xffffff);
				models.buffer(&plane);
			}
	));

	handlers.addHandler(Amber::Handler<SnakeEvents::Move>(
			[&](SnakeEvents::Move& e) {
				auto headT = glm::vec3{};
				switch (e.dir) {
					case LEFT:
						headT = {-10, 0, 0};
						break;
					case RIGHT:
						headT = {10, 0, 0};
						break;
					case UP:
						headT = {0, 10, 0};
						break;
					case DOWN:
						headT = {0, -10, 0};
						break;
				}
				std::list<Model>::iterator it;
				for (it = --segments.end(); it != segments.begin();) {
					auto& segment = *it;
					segment.transform.setTranslation((--it)->transform.getTranslation());
				}
				segments.front().transform.translate(headT);
				models.buffer(&plane);
			}
	));

	handlers.addHandler(Handler<SnakeEvents::Grow>(
			[&](SnakeEvents::Grow& e) {
				auto& segment = segments.emplace_back();
				segment.setMesh(&plane);
				segment.transform.setTranslation({e.x * 10, e.y * 10, -1});
				segment.transform.scale(glm::vec3(5));
				models.addModel(segment);
				colours.push_back((float) 0xff0000 / 0xffffff);
				models.buffer(&plane);
			}
	));

	handlers.addHandler(Handler<SnakeEvents::Fruit>(
			[&](SnakeEvents::Fruit& e) {
				fruit.transform.setTranslation({e.x * 10, e.y * 10, -1});
				models.buffer(&plane);
			}
	));

	handlers.addHandler(Handler<SnakeEvents::Death>(
			[&](SnakeEvents::Death& e) {
				stage.kill();
			}
	));

//	auto& app = static_cast<Application<SnakeGame>&>(stage->engine.application);
//	models.addMesh(stage.assets.getMesh(0), app.R.height * app.R.height + 2);
//
//	static Texture t{Amber::RawTexture{nullptr, Amber::TEXTURE_2D, Amber::SupportedDataFormats::RGBA, 50, 50, 3},
//	                 RGBA8};
//
//	static Framebuffer f;
//
//	f.attachDraw(Amber::SupportedFBOAttachments::COLOUR, *tex);
}

SnakeScene::~SnakeScene() {

}

void SnakeScene::show() {

}

void SnakeScene::hide() {

}

void SnakeScene::update() {
//	if (stage.keys.down[GLFW_KEY_W]) {
//		camera.move({0, 0, 0.1});
//	}
//	if (stage.keys.down[GLFW_KEY_S]) {
//		camera.move({0, 0, -0.1});
//	}
}

void SnakeScene::pick(int x, int y) {

}

void SnakeScene::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 500, 500);
	auto& assets = stage.assets;

	Amber::Mesh* plane = &assets.getMesh(SnakeAssets::PLANE);
	Amber::Shader* shader = &assets.getShader(SnakeAssets::SHADER1);
	shader->start();

	Amber::Texture& yellow = assets.getTexture(SnakeAssets::YELLOW_SQUARE);
	yellow.bindToUnit(GL_TEXTURE0 + 3);
	glUniform1i(22, 3);

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

	glfwSwapBuffers(stage.getWindow());

}
