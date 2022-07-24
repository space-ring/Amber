//
// Created by croissant on 28/05/2022.
//

#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include "maths.h"

namespace Amber {
    typedef struct light {
        glm::vec3 position, colour;
    } Light;
}
#endif //ENGINE_LIGHT_H
