//
// Created by croissant on 05/07/2022.
//

#ifndef ENGINE_DEMO_H
#define ENGINE_DEMO_H


#include "Scene.h"

class Demo : Scene {


    ~Demo() override;

    void setup() override;

    void show() override;

    void hide() override;

    void render() override;
};


#endif //ENGINE_DEMO_H
