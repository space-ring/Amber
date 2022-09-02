//
// Created by croissant on 11/08/2022.
//

#include "scenes/demo/GroundLayer.h"
#include "scenes/demo/DemoScene.h"
#include "Engine.h"
#include "Stage.h"

void GroundLayer::build() {
	Amber::Mesh* mesh = Amber::AssetManager::getInstance().getMesh("plane");
	m1.setMesh(mesh);
	m2.setMesh(mesh);
	m3.setMesh(mesh);
	m4.setMesh(mesh);

	m1.setState(Amber::RenderState::VISIBLE);
	m3.setState(Amber::RenderState::VISIBLE);
	m2.setState(Amber::RenderState::VISIBLE_SOLID);

	m2.getTransform()->attachParent(*m1.getTransform(), false);
	m3.getTransform()->attachParent(*m1.getTransform(), false);
	m4.getTransform()->attachParent(*m1.getTransform(), false);

	m1.translate(glm::vec3(0, 0, -200));
	m2.translate(glm::vec3(2, 0, -1));
	m3.translate(glm::vec3(0, 2, -1));
	m4.translate(glm::vec3(2, 2, -1));

//	m1.setTranslation(glm::vec3(0, 0, -2));

	int x = 100;

	for (int i = -x; i < x; ++i) {
		Amber::Model& row = models.newModel();
		row.setMesh(mesh);
		row.translate(glm::vec3(0, i * 2, 0));
		row.getTransform()->attachParent(*m1.getTransform(), true);
		models.add(row, 250000);
//		for (int j = 0; j < 1; ++j) {
//			Amber::Model& asd = models.newModel();
//			asd.setMesh(mesh);
//			asd.translate(glm::vec3(0, i * 2, 0));
//			models.add(asd, 10000);
//		}
		for (int j = -x; j < x; ++j) {
			Amber::Model& m = models.newModel();
			m.setMesh(mesh);
			m.translate(glm::vec3(j * 2, 0, 0));
			m.getTransform()->attachParent(*row.getTransform(), true);
			models.add(m);
		}
	}

	models.add(m1);
	models.add(m2);
	models.add(m3);
	models.add(m4);
	glCheckError();

//	glBindBuffer(GL_ARRAY_BUFFER, mesh->getInstanceVbo());
//	glBufferData(GL_ARRAY_BUFFER, 100* sizeof (glm::mat4), nullptr, GL_DYNAMIC_DRAW);
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4), &m1.getTransform()->own);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glCheckError();

	ground.setMesh(mesh, 100);
	ground.setRotation(glm::vec3(90, 0, 0));
	ground.setScale(glm::vec3(100));
	ground.setTranslation(glm::vec3(0, -5, 0));

	handlers.addHandler(Amber::window_events::KeyHandler(
			[&](Amber::window_events::KeyEvent& e) {
				if (e.key == GLFW_KEY_UP) {
					models.remove(m2);
				} else if (e.key == GLFW_KEY_DOWN) {
					models.add(m2);
				}
			}
	));

	m1.getTransform()->propagate();
	models.buffer(m1.getMesh());
}

void GroundLayer::show() {

}

void GroundLayer::hide() {

}

void GroundLayer::update() {
//	m1.translate(glm::vec3(0.1/60, 0, 0));
	if (DemoScene::getInstance().keys.down.contains(GLFW_KEY_J)) {
		m1.rotate(glm::vec3(0, 0, 45.0 / 60));
		m1.getTransform()->propagate();
		models.buffer(m1.getMesh());
	}
}

Amber::Model* GroundLayer::pick(int x, int y) {
	return 0;
	Layer::pick(x, y);

	//todo first choose models to render using ray tracing and BBs.
	//todo render to a framebuffer? don't really need to, not swapping buffers here
	//todo example of standardised picking (model manager):
	Amber::Engine& engine = Amber::Engine::getInstance();
	DemoScene& scene = DemoScene::getInstance();
	Amber::Mesh* plane = Amber::AssetManager::getInstance().getMesh("plane");

	Amber::Shader* shader = Amber::AssetManager::getInstance().getShader("pick");
	shader->start();

	glBindVertexArray(plane->getVao());

	glUniformMatrix4fv(14, 1, false, glm::value_ptr(scene.camera.getView()));
	glUniformMatrix4fv(18, 1, false, glm::value_ptr(scene.camera.getPerspective()));

	int offset = 1;
	glUniform1i(19, offset);

	glDrawElementsInstanced(GL_TRIANGLES, plane->getElementCount(), GL_UNSIGNED_INT, nullptr,
	                        models.getPickCount(plane));

	glBindVertexArray(0);
	shader->stop();

	//todo omit these?
	//todo read pixel colour & check with manager
//	glFlush();
//	glFinish();
//
//	int colour_picked[3];
//	glReadPixels(x, y, 1, 1, GL_RGB, GL_INT, &colour_picked);
//	std::cout << "picked " << x << " " << y << " " << colour_picked[0] << " " << colour_picked[1] << " "
//	          << colour_picked[2] << " " << std::endl;
//
//	glfwSwapBuffers(engine.stage->getWindow());

	offset += models.getPickCount(plane);
	//render next mesh

	return nullptr;
}

void GroundLayer::render() {
	Amber::Engine& engine = Amber::Engine::getInstance();
	DemoScene& scene = DemoScene::getInstance();
	Amber::Mesh* plane = Amber::AssetManager::getInstance().getMesh("plane");

	Amber::Shader* shader = Amber::AssetManager::getInstance().getShader("basic")->start();

	glBindVertexArray(plane->getVao());
	glCheckError();
	glUniformMatrix4fv(14, 1, false, glm::value_ptr(scene.camera.getView()));
	glUniformMatrix4fv(18, 1, false, glm::value_ptr(scene.camera.getPerspective()));

	glDrawElementsInstanced(GL_TRIANGLES, plane->getElementCount(), GL_UNSIGNED_INT, nullptr,
	                        models.getRenderCount(plane));
	glCheckError();
	glBindVertexArray(0);
	shader->stop();
}
