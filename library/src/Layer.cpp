//
// Created by croissant on 11/07/2022.
//

#include "Layer.h"

namespace Amber {

    void Layer::removeHandler(long id) {
        handlers.erase(id);
    }

}