//
// Created by croissant on 11/07/2022.
//

#ifndef ENGINE_LAYER_H
#define ENGINE_LAYER_H

#include "event.h"
#include "ModelManager.h"
#include <map>

namespace Amber {
    class Layer {
        std::map<unsigned long, GenericHandler> handlers;

    public:

        ModelManager models;

        bool active = true;

        Layer() = default;

        virtual ~Layer() = default;

        virtual void render() = 0;

        /*
         * todo default implementation,
         * perhaps store all pickable models in a list (map by mesh) what about fbos?
         * Remember to check against frame bounds in subs!
         */
        virtual void pick(double x, double y) = 0;

        virtual void update() = 0;

        template<class T>
        void addHandler(const EventHandler<T>& handler) {
            if (handlers.contains(handler.type)) {
                handlers.erase(handler.type);
            }
            handlers.insert(std::pair(handler.type, EventHandler<T>::upcast(handler)));
        }

        template<class T>
        void onEvent(T& event) {
            long id = EventHandler<T>::type;
            if (!handlers.contains(id)) return;
            handlers.at(id)(event);
        }

        void removeHandler(long id);

    };

}


#endif //ENGINE_LAYER_H
