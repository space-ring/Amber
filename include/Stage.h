//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_STAGE_H
#define ENGINE_STAGE_H

#include "graphics.h"
#include <string>
#include "event.h"
#include <list>
#include <queue>
#include <map>

class Stage {
    using string = std::string;
    template<class T>
    using vector = std::vector<T>;

private:
    string name;
    int x, y, width, height;
    GLFWwindow* window = nullptr;
    bool focused;

    //handlers
    std::map<
            long,
            vector<window_event::GenericHandler>*
    >* handlers = new std::map<
            long,
            vector<window_event::GenericHandler>*
    >;

    void render();

    void poll();

    void update();

public:

    Stage(const string& name, int x, int y, int width, int height);

    virtual ~Stage();

    /* Initialises GLFW window and OpenGL context */
    void init();

    /* Enters the main render loop */
    void run();

    void terminate();

    void show();

    void hide();

    GLFWwindow* getWindow() const;

    // add any handler
    template<class T>
    void addHandler(const window_event::EventHandler<T>& handler) {
        if (!handlers->contains(handler.type)) {
            handlers->insert(std::pair(handler.type, new vector<window_event::GenericHandler>));
        }
        auto* list = handlers->at(handler.type);
        //upcast to change template parameter (T is any here)
        list->push_back(window_event::GenericHandler::upcast(handler));
    }

    void clearHandlers(long id);

    // call any handler
    /*
     * handler is GenericHandler so event parameter enforces T:Event
     * although handler(event) below enforces T:Event, keep template to find id.
     * todo unless events hold id and handlers reference them
     * using static cast to avoid accidental overheads
     */
    template<class T>
    void onEvent(const T& event) {
        long id = window_event::EventHandler<T>::type;
        if (!handlers->contains(id)) return;
        for (auto& handler: *handlers->at(id)) {
            handler(static_cast<const window_event::Event&>(event));
        }
    }

    /*
     * call to onEvent enforces T:Event
     * using static cast to avoid accidental overheads
     */
    template<class T, class... Args>
    static void onGLFWevent(GLFWwindow* window, Args... args) {
        auto* stage = static_cast<Stage*>(glfwGetWindowUserPointer(window));
        stage->template onEvent(static_cast<const window_event::Event&>(T(window, args...)));
    }
};


#endif //ENGINE_STAGE_H
