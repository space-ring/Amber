//
// Created by croissant on 01/04/2022.
//

#ifndef ENGINE_SHADER_H
#define ENGINE_SHADER_H


#include <string>
#include "graphics.h"

using source = const char *const *; //todo scope?

class Shader {
private:
    GLuint program, vertex, tessCtrl, tessEval, geometry, fragment, compute;
    static Shader *DEFAULT;

public:
    Shader(GLuint program, GLuint vertex, GLuint tessCtrl, GLuint tessEval, GLuint geometry, GLuint fragment, GLuint compute);

    Shader(source vertex, source tessCtrl, source tessEval, source geometry, source fragment, source compute);

    virtual ~Shader();

    static Shader *getDefault();

    GLuint getProgram() const;

};

extern const GLchar *const *DEFAULT_VERTEX;
extern const GLchar *const *DEFAULT_FRAGMENT;


#endif //ENGINE_SHADER_H
