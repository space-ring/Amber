//
// Created by Chris on 01/07/2022.
//

#ifndef ENGINE_RENDERING_H
#define ENGINE_RENDERING_H

#include <functional>
#include "Frame.h"

class Scene;

class Engine;

template<class... Args>
class RenderCall {
    using f = std::function<Frame(Args...)>;

    const f function;

public:
    RenderCall(const f& function) : function(function) {}

    Frame operator()(Args... args) {
        return function(args...);
    }
};

using GenericLens = RenderCall<const Engine&, const Scene&>;

extern const GenericLens DEFAULT_WIRE;

extern const GenericLens DEFAULT_PICKER;

#endif //ENGINE_RENDERING_H
