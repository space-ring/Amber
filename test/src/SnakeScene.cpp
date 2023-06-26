//
// Created by croissant on 01/09/2022.
//

#include "scenes/demo/SnakeScene.h"
#include "snake.h"
#include "Application.h"
#include "Texture.h"

SnakeScene::SnakeScene(Stage& stage, unsigned int imWidth, unsigned int imHeight)
		: Scene(stage), camera(glm::vec3(0), glm::vec3(0), 70, imWidth, imHeight, 0, 100, 0, 100) {

	handlers.addHandler(Amber::window_events::KeyHandler(
			[&](Amber::window_events::KeyEvent& e) {
				auto& app = stage.eventOutStream;
				if (e.action == 1) {
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
				if (e.key == GLFW_KEY_G && e.action > 0) {
					app.putEvent(SnakeEvents::CheatGrow{});
				}

				if (e.key == GLFW_KEY_ESCAPE) {
					stage.handlers.onEvent(window_events::CloseEvent{stage.getWindow()});
				}
			}
	));

	auto& plane = stage.assets.getMesh(0);
	models.addMesh(&plane, 50 * 50 + 2);


	handlers.addHandler(Amber::Handler<SnakeEvents::Spawn>(
			[&](SnakeEvents::Spawn& e) {
//				auto& segment = segments.emplace_front();
//				segment.setMesh(&plane);
//				segment.transform.translate(glm::vec3(e.x, e.y, -1));
//				segment.transform.scale(glm::vec3(0.5));
//				models.addModel(segment);
//				colours.push_back((float) 0xff0000 / 0xffffff);
//				std::cout << "spawned" << std::endl;
//				models.buffer(&plane);

				auto& m = models.newModel();
				m.setMesh(&plane);
				models.addModel(m);
				m.transform.setTranslation({25, 25, -1});
				models.buffer(&plane);
			}
	));

	handlers.addHandler(Amber::Handler<SnakeEvents::Move>(
			[&](SnakeEvents::Move& e) {
				return;
				auto headT = glm::vec3{};
				switch (e.dir) {
					case LEFT:
						headT = {-1, 0, 0};
						break;
					case RIGHT:
						headT = {1, 0, 0};
						break;
					case UP:
						headT = {0, 1, 0};
						break;
					case DOWN:
						headT = {0, -1, 0};
						break;
				}
				segments.front().transform.translate(headT);
				std::cout << "move" << std::endl;
				std::list<Model>::iterator it;
				auto& current = segments.front().transform;
				for (it = --segments.end(); it != segments.begin(); --it) {
					auto& segment = *it;
					segment.transform.setTranslation(current.getTranslation());
				}
				models.buffer(&stage.assets.getMesh(0));
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
	//send key events
//	auto& app = static_cast<Application<SnakeGame>&>(stage->engine.application);
//
//	SnakeGame::R& game = app.R;
//	segments.clear();
//	colours.clear();
//
//	Amber::Mesh& plane = stage.assets.getMesh(0);
//
//	for (auto& p: game.snake.segments) {
//		auto& segment = segments.emplace_back();
//		segment.setMesh(&plane);
//		segment.transform.translate(glm::vec3(p.x, p.y, -1));
//		segment.transform.scale(glm::vec3(0.5));
//		models.addModel(segment);
//		colours.push_back((float) 0xff0000 / 0xffffff);
//	}
//
//	auto& fruit = segments.emplace_back();
//	fruit.setMesh(&plane);
//	fruit.transform.scale(glm::vec3(0.5));
//	fruit.transform.translate(glm::vec3(game.fruit.x, game.fruit.y, -1));
//	models.addModel(fruit);
//
//	models.buffer(&plane);
}

void SnakeScene::pick(int x, int y) {

}

void SnakeScene::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 500, 500);
	auto& assets = stage.assets;

	Amber::Mesh* plane = &assets.getMesh(0);
	Amber::Shader* shader = &assets.getShader(0);
	shader->start();

	Amber::Texture& yellow = assets.getTexture(0);
	yellow.bindToUnit(GL_TEXTURE0+3);
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
