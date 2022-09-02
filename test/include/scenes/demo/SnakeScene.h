//
// Created by croissant on 01/09/2022.
//

#ifndef ENGINETEST_SNAKESCENE_H
#define ENGINETEST_SNAKESCENE_H

#include "Scene.h"
#include "NoDefaultSingleton.h"

using namespace Amber;

class SnakeScene : public Scene, public NoDefaultSingleton<SnakeScene> {

	friend Singleton<SnakeScene>;

	Camera camera;

	SnakeScene();

	~SnakeScene();

	void controlCamera();

public:
	void build() override;

	void show() override;

	void hide() override;

	void update() override;

	void pick(int x, int y) override;

	void render() override;
};


#endif //ENGINETEST_SNAKESCENE_H
