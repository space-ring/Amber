//
// Created by Chris on 30/06/2022.
//

#ifndef ENGINE_EVENTMANAGER_H
#define ENGINE_EVENTMANAGER_H

#include <map>
#include <vector>
#include "event.h"
#include "Engine.h"

class EventManager {
    using handlerMap = std::map<long, std::vector<window_events::GenericHandler>*>;
    handlerMap* handlers = new handlerMap;

public:
    virtual ~EventManager();

public:
    // add any handler
    template<class T>
    void addHandler(const window_events::EventHandler<T>& handler) {
        if (!handlers->contains(handler.type))
            handlers->insert(std::pair(handler.type, new std::vector<window_events::GenericHandler>));

        auto* list = handlers->at(handler.type);
        //upcast to change template parameter (DerivedEvent is any here)
        list->push_back(window_events::EventHandler<T>::upcast(handler));
    }

    void clearHandlers(long id);

    // call any handler
    /*
     * handler is GenericHandler so event parameter enforces DerivedEvent:BaseEvent
     * although handler(event) below enforces DerivedEvent:BaseEvent, keep template to find id.
     * handler call is coerced (compile time)
     * todo unless events hold id and handlers reference them
     */
    template<class T>
    void onEvent(const T& event) {
        long id = window_events::EventHandler<T>::type;
        if (!handlers->contains(id)) return;
        for (auto& handler: *handlers->at(id)) {
            handler(event);
        }
    }

    /*
     * call to onEvent enforces DerivedEvent:BaseEvent
     */
    template<class T, class... Args>
    static void onGLFWevent(GLFWwindow* window, Args... args) {
        auto* p = static_cast<Engine*>(glfwGetWindowUserPointer(window));
        p->events->template onEvent(T{window, args...});
    }
};


#endif //ENGINE_EVENTMANAGER_H
