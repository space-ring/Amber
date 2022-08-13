//
// Created by croissant on 11/07/2022.
//

#ifndef ENGINE_LAYER_H
#define ENGINE_LAYER_H

#include "event.h"
#include "ModelManager.h"
#include "EventManager.h"
#include <map>

namespace Amber {
    class Layer {

    public:

        ModelManager models;
        EventManager handlers;

        bool active = true;

        Layer() = default;

        virtual ~Layer() = default;

        virtual void render() = 0;

        virtual Model* pick(double x, double y) = 0;

        virtual void update() = 0;

    };

}


#endif //ENGINE_LAYER_H
