//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_STAGE_H
#define ENGINE_STAGE_H

#include "graphics.h"
#include <string>

class Stage {

private:
    const char *name;
    int x, y, width, height;
    GLFWwindow *window;

public:
    Stage(const char *name, int x, int y, int width, int height);

    virtual ~Stage();

    void init();

    void run();

    void terminate();

    void show();

    void hide();
};


#endif //ENGINE_STAGE_H
