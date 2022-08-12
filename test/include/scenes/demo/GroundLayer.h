//
// Created by croissant on 11/08/2022.
//

#ifndef ENGINETEST_GROUNDLAYER_H
#define ENGINETEST_GROUNDLAYER_H

#include "Layer.h"

class GroundLayer : public Amber::Layer{
    friend class DemoScene;

    Amber::Model m1, m2, m3, m4;

public:

    GroundLayer();

    void render() override;

    void pick(double x, double y) override;

    void update() override;
};


#endif //ENGINETEST_GROUNDLAYER_H
