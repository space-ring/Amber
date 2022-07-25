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
#include "Engine.h"
#include "EventManager.h"

namespace Amber {
    class Stage {
        using string = std::string;

    private:
        Engine* root;
        string name;
        int x, y, width, height;
        GLFWwindow* window = nullptr;
        std::map<string, Scene*>* scenes = new std::map<string, Scene*>;
        Scene* front = nullptr;

        template<class T, class... Args>
        static void onGLFWevent(GLFWwindow* window, Args... args) {
            auto* p = static_cast<Engine*>(glfwGetWindowUserPointer(window));
            T event{window, args...};
            if (p->getStage()->getFront()) {
                p->getStage()->getFront()->template onEvent(event);
                if (!event.handled)
                    p->events->template onEvent(event);
            } else
                p->events->template onEvent(event);
        }

    public:

        bool focused;

        Stage(Engine* root, const string& name, int x, int y, int width, int height);

        virtual ~Stage();

        /* Initialises GLFW window and OpenGL context */
        void init();

        void render();

        void poll();

        void update();

        void pick();

        void show();

        void hide();

        GLFWwindow* getWindow() const;

        void addScene(const string& id, Scene* scene);

        void setFrontScene(const string& scene);

        Scene* getFront() const;

        Engine* getRoot() const;
    };

}

#endif //ENGINE_STAGE_H
