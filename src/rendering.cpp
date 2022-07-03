//
// Created by Chris on 01/07/2022.
//

#include <iostream>
#include "rendering.h"
#include "Engine.h"
#include "Scene.h"

const GenericLens DEFAULT_WIRE(
        [](const Engine& engine, const Scene& scene) {
            std::cout << "Wire lens not implemented yet" << std::endl;
            return Frame();
        }
);

const GenericLens DEFAULT_PICKER(
        [](const Engine& engine, const Scene& scene) {
            std::cout << "Wire lens not implemented yet" << std::endl;
            return Frame();
        }
);
