//
// Created by croissant on 01/04/2022.
//

#ifndef ENGINE_SHADER_H
#define ENGINE_SHADER_H


#include <string>
#include "graphics.h"

struct compoundShader { //todo const
    std::string vertex, fragment, geometry, tessControl, tessEval, compute;
};

enum SupportedShaders {
    VERTEX = GL_VERTEX_SHADER,
    TESS_CONTROL = GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    COMPUTE = GL_COMPUTE_SHADER
};

class Shader {
private:
    static Shader* DEFAULT;

    compoundShader* sources; //todo delete

    GLuint program = 0,
            vertex = 0,
            tessCtrl = 0,
            tessEval = 0,
            geometry = 0,
            fragment = 0,
            compute = 0;

    GLuint addShader(SupportedShaders type, const GLchar* const* code);

public:
    static Shader* getDefault();

    Shader(GLuint program, GLuint vertex, GLuint tessCtrl, GLuint tessEval, GLuint geometry, GLuint fragment,
           GLuint compute);

    Shader(compoundShader* sources);

    virtual ~Shader();

    Shader* build();

    void start();

    void stop();
};

#endif //ENGINE_SHADER_H
