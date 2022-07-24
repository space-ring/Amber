//
// Created by croissant on 18/07/2022.
//

#ifndef ENGINE_LAYERSTACK_H
#define ENGINE_LAYERSTACK_H


#include <vector>
#include "Layer.h"

namespace Amber {
    class LayerStack {

    private:
        std::vector<Layer*> stack;

    public:
        LayerStack();

        virtual ~LayerStack();

        void render();

        template<class T>
        void onEvent(T& event){
            for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
                if (event.handled) break;
                if (!(*it)->active) continue;
                (*it)->onEvent(event);
            }
        }

        void pick(double x, double y);

        void update();

        void addLayer(Layer* layer);

        void removeLayer(Layer* layer);

        std::vector<Layer*>::iterator begin();

        std::vector<Layer*>::iterator end();

        std::vector<Layer*>::reverse_iterator rbegin();

        std::vector<Layer*>::reverse_iterator rend();

        std::vector<Layer*>::const_iterator begin() const;

        std::vector<Layer*>::const_iterator end() const;

        std::vector<Layer*>::const_reverse_iterator rbegin() const;

        std::vector<Layer*>::const_reverse_iterator rend() const;
    };

}

#endif //ENGINE_LAYERSTACK_H
