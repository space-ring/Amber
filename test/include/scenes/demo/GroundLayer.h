//
// Created by croissant on 11/08/2022.
//

#ifndef ENGINETEST_GROUNDLAYER_H
#define ENGINETEST_GROUNDLAYER_H

#include "Layer.h"

class GroundLayer : public Amber::Layer {
	friend class DemoScene;

	Amber::Model ground;
	Amber::Model m1, m2, m3, m4;

public:

	GroundLayer() = default;

	void build() override;

	void show() override;

	void hide() override;

	void update() override;

	Amber::Model* pick(int x, int y) override;

	void render() override;
};


#endif //ENGINETEST_GROUNDLAYER_H
