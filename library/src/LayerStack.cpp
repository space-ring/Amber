//
// Created by croissant on 18/07/2022.
//

#include "LayerStack.h"

namespace Amber {
    LayerStack::LayerStack() = default;

    LayerStack::~LayerStack() = default;

    void LayerStack::render() {
        for (auto layer : stack) {
            if (!layer->active) continue;
            layer->render();
        }
    }

    Model* LayerStack::pick(double x, double y) {
        Model* picked = nullptr;
        for (auto layer : stack) {
            if (!layer->active) continue;
            picked = layer->pick(x,y);
            if (picked) break;
        }
        return picked;
    }

    void LayerStack::update() {
        for (auto layer : stack) {
            if (!layer->active) continue;
            layer->update();
        }
    }

    void LayerStack::addLayer(Layer* layer) {
        stack.push_back(layer);
    }

    void LayerStack::removeLayer(Layer* layer) {
        auto it = std::find(stack.begin(), stack.end(), layer);
        if (it != stack.end()) {
            stack.erase(it);
        }
    }

    std::vector<Layer*>::iterator LayerStack::begin() { return stack.begin(); }

    std::vector<Layer*>::iterator LayerStack::end() { return stack.end(); }

    std::vector<Layer*>::reverse_iterator LayerStack::rbegin() { return stack.rbegin(); }

    std::vector<Layer*>::reverse_iterator LayerStack::rend() { return stack.rend(); }

    std::vector<Layer*>::const_iterator LayerStack::begin() const { return stack.begin(); }

    std::vector<Layer*>::const_iterator LayerStack::end() const { return stack.end(); }

    std::vector<Layer*>::const_reverse_iterator LayerStack::rbegin() const { return stack.rbegin(); }

    std::vector<Layer*>::const_reverse_iterator LayerStack::rend() const { return stack.rend(); }

}