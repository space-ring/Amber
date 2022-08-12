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

    float r = 0, g = 0, b = 0, a = 1;
    Amber::window_events::FocusHandler onFocus;
    Amber::window_events::EnterHandler onEnter;

    Amber::Model cube, triangle;

    void test();

public:
    DemoLayer();

    void render() override;

    void pick(double x, double y) override;

    void update() override;

};

#endif //ENGINETEST_DEMOLAYER_H
