//
// Created by Chris on 06/04/2022.
//

#ifndef ENGINE_EVENT_H
#define ENGINE_EVENT_H

namespace scene_event {


}

namespace context_event {
    struct enterEvent {
        GLFWwindow *window;
        int entered;
    };

    struct focusEvent {
        GLFWwindow *window;
        int focused;
    };

    struct keyEvent {
        GLFWwindow *window;
        int key;
        int scancode;
        int action;
        int mods;
    };

    struct charEvent {
        GLFWwindow *window;
        unsigned int codepoint;
    };

    struct motionEvent {
        GLFWwindow *window;
        double xpos;
        double ypos;
    };

    struct clickEvent {
        GLFWwindow *window;
        int button;
        int action;
        int mods;
    };

    struct scrollEvent {
        GLFWwindow *window;
        double xoffset;
        double yoffset;
    };

    //todo joystick, gamepad

    typedef void (*focusHandler)(struct focusEvent);

    typedef void (*enterHandler)(struct enterEvent);

    typedef void (*keyHandler)(struct keyEvent);

    typedef void (*charHandler)(struct charEvent);

    typedef void (*clickHandler)(struct clickEvent);

    typedef void (*scrollHandler)(struct scrollEvent);

    typedef void (*motionHandler)(struct motionEvent);
}


#endif //ENGINE_EVENT_H
