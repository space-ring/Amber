//
// Created by croissant on 01/04/2022.
//

#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"
#include "engineIO.h"

//todo separate programs and shaders?
namespace Amber {

    string DEFAULT_VERTEX =
            "#version 430 core\n"
            "layout (location = 0) in vec3 v_position;\n"
            "layout (location = 1) in vec2 v_colour;\n"
            "layout (location = 2) in vec3 v_normal;\n"
            "\n"
            "layout (location = 10) in mat4 M;\n"
            "\n"
            "layout (location = 14) uniform mat4 V;\n"
            "layout (location = 18) uniform mat4 P;\n"
            "\n"
            "uniform sampler2D tex;\n"
            "\n"
            "out vec3 frag_position;\n"
            "out vec3 frag_normal;\n"
            "out vec2 tex_coords;\n"
            "\n"
            "void main(void){\n"
            "    mat4 N = transpose(inverse(V * M));\n"
            "    frag_position = vec3(M * vec4(v_position, 1.0));\n"
            "    frag_normal = normalize(vec3(N * vec4(v_normal, 1.0)));\n"
            "    tex_coords = v_colour;\n"
            "\n"
            "    gl_Position = P * V * vec4(frag_position, 1.0);\n"
            "}";


    string DEFAULT_FRAGMENT =
            "#version 430 core\n"
            "\n"
            "in vec3 frag_position;\n"
            "in vec3 frag_normal;\n"
            "in vec2 tex_coords;\n"
            "\n"
            "layout (location = 22) uniform vec3 light_position;\n"
            "layout (location = 23) uniform vec3 light_colour;\n"
            "layout (location = 24) uniform float ambient;\n"
            "\n"
            "uniform sampler2D tex;\n"
            "\n"
            "out vec4 fragment;\n"
            "\n"
            "void main(void){\n"
            "\n"
            "    vec3 ambient_filter = ambient * light_colour;\n"
            "\n"
            "    vec3 to_light = normalize(light_position - frag_position);\n"
            "    float brightness = max(0.0, dot(frag_normal, to_light));\n"
            "    vec3 diffuse_filter = brightness * light_colour;\n"
            "\n"
            "    vec4 pixel = texture(tex, tex_coords);\n"
            "    if (pixel.a < 1){\n"
            "        discard;\n"
            "    }\n"
            "    vec4 sum_filter = vec4(max(ambient_filter, diffuse_filter), 1.0);\n"
            "    fragment = pixel * sum_filter;\n"
            "}";


    Shader* Shader::DEFAULT = nullptr;

    Shader* Shader::getDefault() { //todo upcast for GL context
        if (!DEFAULT) {
            compoundShader defaultSources{DEFAULT_VERTEX, DEFAULT_FRAGMENT};
            DEFAULT = loadShader(defaultSources);
        }
        return DEFAULT;
    }

    void Shader::deleteDefault() {
        if (DEFAULT) delete DEFAULT;
    }

    Shader::Shader(GLuint program, GLuint vertex, GLuint tessCtrl, GLuint tessEval, GLuint geometry, GLuint fragment,
                   GLuint compute)
            : sources(new compoundShader{}), program(program), vertex(vertex), tessCtrl(tessCtrl),
              tessEval(tessEval), geometry(geometry), fragment(fragment), compute(compute) {
    }

    Shader::Shader(compoundShader* sources) : sources(sources) {
    }

    Shader::~Shader() {
        //delete gl objects
        if (!program) return;
        glDeleteProgram(program); // also detaches shaders
        glDeleteShader(vertex);
        glDeleteShader(tessCtrl);
        glDeleteShader(tessEval);
        glDeleteShader(geometry);
        glDeleteShader(fragment);
        glDeleteShader(compute);
        delete sources;
    }

    void Shader::start() {
        glUseProgram(program);
    }

    void Shader::stop() {
        glUseProgram(0);
    }

    GLuint Shader::addShader(SupportedShaders type, const GLchar* const* code) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, code, nullptr);
        glCompileShader(shader);
        GLint flag;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &flag);
        if (!flag) {
            GLint logSize = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
            std::vector<GLchar> log(logSize);
            glGetShaderInfoLog(shader, logSize, nullptr, &log[0]);
            // todo logging
            std::cout << "Failed to compile shader " << type << std::endl;
            int i = 0;
            while (*(*code + i) != '\0') {
                std::cout << *(*code + i++);
            }
            std::cout << std::endl;
            for (GLchar c: log) {
                std::cout << c;
            }
            glDeleteShader(shader);
            return 0;
        }

        switch (type) {
            case VERTEX:
                vertex = shader;
                break;
            case TESS_CONTROL:
                tessCtrl = shader;
                break;
            case TESS_EVALUATION:
                tessEval = shader;
                break;
            case GEOMETRY:
                geometry = shader;
                break;
            case FRAGMENT:
                fragment = shader;
                break;
            case COMPUTE:
                compute = shader;
                break;
        }

        return shader;
    }

    Shader* Shader::build() {
        if (program) return this;

        if (!sources->vertex.empty()) {
            const char* p = sources->vertex.c_str();
            addShader(VERTEX, &p);
        }
        if (!sources->tessControl.empty()) {
            const char* p = sources->tessControl.c_str();
            addShader(TESS_CONTROL, &p);
        }
        if (!sources->tessEval.empty()) {
            const char* p = sources->tessEval.c_str();
            addShader(TESS_EVALUATION, &p);
        }
        if (!sources->geometry.empty()) {
            const char* p = sources->geometry.c_str();
            addShader(GEOMETRY, &p);
        }
        if (!sources->fragment.empty()) {
            const char* p = sources->fragment.c_str();
            addShader(FRAGMENT, &p);
        }
        if (!sources->compute.empty()) {
            const char* p = sources->compute.c_str();
            addShader(COMPUTE, &p);
        }

        GLuint p = glCreateProgram();
        glAttachShader(p, vertex);
        glAttachShader(p, tessCtrl);
        glAttachShader(p, tessEval);
        glAttachShader(p, geometry);
        glAttachShader(p, fragment);
        glAttachShader(p, compute);

        //todo pre-linkup build e.g. transform feedback
        glLinkProgram(p);

        GLint flag;
        glGetProgramiv(p, GL_LINK_STATUS, &flag);
        if (!flag) {
            GLint logSize = 0;
            glGetProgramiv(p, GL_INFO_LOG_LENGTH, &logSize);
            std::vector<GLchar> log(logSize);
            glGetProgramInfoLog(p, logSize, &logSize, &log[0]);
            //todo logging
            std::cout << "Error creating shader program" << std::endl;
            for (GLchar c: log) {
                std::cout << c;
            }
            glDeleteProgram(p);
            glDeleteShader(vertex);
            glDeleteShader(tessCtrl);
            glDeleteShader(tessEval);
            glDeleteShader(geometry);
            glDeleteShader(fragment);
            glDeleteShader(compute);
            program = 0;
            vertex = 0;
            tessCtrl = 0;
            tessEval = 0;
            geometry = 0;
            fragment = 0;
            compute = 0;
            return this;
        }

        program = p;
        return this;
    }

}