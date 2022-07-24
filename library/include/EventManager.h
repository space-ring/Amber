//
// Created by Chris on 30/06/2022.
//

#ifndef ENGINE_EVENTMANAGER_H
#define ENGINE_EVENTMANAGER_H

#include <map>
#include <vector>
#include "event.h"

namespace Amber {
    class EventManager {
        using handlerMap = std::map<unsigned long, std::vector<GenericHandler>*>;
        handlerMap* handlers = new handlerMap();

    public:
        // add any handler
        template<class T>
        void addHandler(const EventHandler<T>& handler) {
            if (!handlers->contains(handler.type))
                handlers->insert(std::pair(handler.type, new std::vector<GenericHandler>));

            auto* list = handlers->at(handler.type);
            //upcast to change template parameter (T is any here)
            list->push_back(EventHandler<T>::upcast(handler));
        }

        void clearHandlers(unsigned long id);

        // call any handler
        /*
         * handler is GenericHandler so event parameter enforces T:Event
         * although handler(event) below enforces T:Event, keep template to find id.
         * todo unless game_events hold id and handlers reference them (removes template)
         */
        template<class T>
        void onEvent(T& event) {
            long id = EventHandler<T>::type;
            if (!handlers->contains(id)) return;
            for (auto& handler: *handlers->at(id)) {
                handler(event);
            }
        }

        /*
         * call to onEvent enforces T:Event
         */

    };

}

#endif //ENGINE_EVENTMANAGER_H
