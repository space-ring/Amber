//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_STAGE_H
#define ENGINE_STAGE_H

#include "graphics.h"
#include <string>
#include "event.h"
#include <list>

class Stage {
    using string = std::string;
private:
    static Stage* mainStage;
    string name;
    int x, y, width, height;
    GLFWwindow* window;

    //handlers
    std::list<context_event::focusHandler>* onFocusHandlers;
    std::list<context_event::enterHandler>* onEnterHandlers;
    std::list<context_event::keyHandler>* onKeyHandlers;
    std::list<context_event::charHandler>* onCharHandlers;
    std::list<context_event::clickHandler>* onClickHandlers;
    std::list<context_event::scrollHandler>* onScrollHandlers;
    std::list<context_event::motionHandler>* onMotionHandlers;

    void render();

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

    // add events
    void addOnStageFocus(context_event::focusHandler handler);

    void addOnStageEnter(context_event::enterHandler handler);

    void addOnKey(context_event::keyHandler handler);

    void addOnChar(context_event::charHandler handler);

    void addOnClick(context_event::clickHandler handler);

    void addOnScroll(context_event::scrollHandler handler);

    void addOnMotion(context_event::motionHandler handler);

    // clear events
    void clearOnFocus();

    void clearOnEnter();

    void clearOnKey();

    void clearOnChar();

    void clearOnClick();

    void clearOnScroll();

    void clearOnMotion();

    // on events
    static void onFocus(GLFWwindow* window, int focused);

    static void onEnter(GLFWwindow* window, int entered);

    static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);

    static void onChar(GLFWwindow* window, unsigned int codepoint);

    static void onClick(GLFWwindow* window, int button, int action, int mods);

    static void onScroll(GLFWwindow* window, double xoffset, double yoffset);

    static void onMotion(GLFWwindow* window, double xpos, double ypos);

};


#endif //ENGINE_STAGE_H
