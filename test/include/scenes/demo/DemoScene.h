//
// Created by croissant on 05/07/2022.
//

#ifndef ENGINE_DEMOSCENE_H
#define ENGINE_DEMOSCENE_H


#include "Scene.h"
#include "Engine.h"
#include "GroundLayer.h"
#include "Singleton.h"

class DemoScene : public Amber::Scene, public Singleton<DemoScene> {
	friend Singleton<DemoScene>;
	friend GroundLayer;

	GroundLayer groundLayer;

	float cameraSpeed = 5.0 / 60;

	Amber::Camera camera;

	DemoScene();

	~DemoScene() override;

	void cameraControl();

public:

	void build() override;

	void show() override;

	void hide() override;

	void update() override;

	void pick(int x, int y) override;

	void render() override;
};


#endif //ENGINE_DEMOSCENE_H
