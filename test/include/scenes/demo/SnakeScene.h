//
// Created by croissant on 01/09/2022.
//

#ifndef ENGINETEST_SNAKESCENE_H
#define ENGINETEST_SNAKESCENE_H

#include "Scene.h"
#include "NoDefaultSingleton.h"
#include "Framebuffer.h"

using namespace Amber;

class SnakeScene : public Amber::Scene {

	Camera camera;

	std::list<Model> segments;

	std::vector<float> colours;

public:

	SnakeScene(Stage& stage, unsigned int imWidth, unsigned int imHeight);

	~SnakeScene();

	void show() override;

	void hide() override;

	void update() override;

	void pick(int x, int y) override;

	void render() override;
};


#endif //ENGINETEST_SNAKESCENE_H
