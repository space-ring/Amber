//
// Created by croissant on 19/07/2022.
//

#ifndef ENGINETEST_DEMOLAYER_H
#define ENGINETEST_DEMOLAYER_H

#include "Layer.h"
#include "Model.h"
#include "Camera.h"


class DemoLayer : public Amber::Layer {
    friend class DemoScene;

    Amber::Model cube;

public:
    DemoLayer();

    void render() override;

    Amber::Model* pick(double x, double y) override;

    void update() override;

};

#endif //ENGINETEST_DEMOLAYER_H
