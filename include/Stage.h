//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_STAGE_H
#define ENGINE_STAGE_H

#include "graphics.h"
#include <string>
#include "event.h"

class Stage {

private:
    static Stage* mainStage;

    const char *name;
    int x, y, width, height;
    GLFWwindow *window;
    context_event::focusHandler onFocusHandler;

    void render();

public:
    Stage(const char *name, int x, int y, int width, int height);

    virtual ~Stage();

    /* Initialises GLFW window and OpenGL context */
    void init();

    /* Enters the main render loop */
    void run();

    void terminate();

    void show();

    void hide();

    GLFWwindow *getWindow() const;

    // add events
    void addOnFocus(void (*handler)(context_event::focusEvent));

    // clear events
    void clearOnFocus();

    // on events
    static void onFocus(GLFWwindow *window, int focused);
    static void onEnter(GLFWwindow* window, int entered);
    static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void onChar(GLFWwindow* window, unsigned int codepoint);
    static void onClick(GLFWwindow* window, int button, int action, int mods);
    static void onScroll(GLFWwindow* window, double xoffset, double yoffset);
    static void onMotion(GLFWwindow* window, double xpos, double ypos);

};


#endif //ENGINE_STAGE_H
