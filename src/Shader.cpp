//
// Created by croissant on 01/04/2022.
//

#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"
#include "io.h"

const char *default_vertex =
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


const char *default_fragment =
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

const GLchar *const *DEFAULT_VERTEX = &default_vertex;
const GLchar *const *DEFAULT_FRAGMENT = &default_fragment;

Shader *Shader::DEFAULT = nullptr;

Shader *Shader::getDefault() { //todo check for GL context
    if (DEFAULT) {
        return DEFAULT;
    } else {
        DEFAULT = new Shader(DEFAULT_VERTEX, nullptr, nullptr, nullptr, DEFAULT_FRAGMENT, nullptr);
    }
    return DEFAULT;
}

Shader::~Shader() {
    //delete gl objects
    glDeleteProgram(program); // also detaches shaders
    glDeleteShader(vertex);
    glDeleteShader(tessCtrl);
    glDeleteShader(tessEval);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
    glDeleteShader(compute);
}

Shader::Shader(GLuint program, GLuint vertex, GLuint tessCtrl, GLuint tessEval, GLuint geometry, GLuint fragment,
               GLuint compute)
        : program(program), vertex(vertex), tessCtrl(tessCtrl),
          tessEval(tessEval), geometry(geometry), fragment(fragment), compute(compute) {}

Shader::Shader(source vertex, source tessCtrl, source tessEval, source geometry, source fragment, source compute) :
        program(0), vertex(0), tessCtrl(0), tessEval(0), geometry(0), fragment(0), compute(0) {

    source sources[]{vertex, tessCtrl, tessEval, geometry, fragment, compute};
    unsigned int types[]{GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER,
                         GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER};
    unsigned int *shaders[]{&this->vertex, &this->tessCtrl, &this->tessEval,
                            &this->geometry, &this->fragment, &this->compute};

    bool compiled = true;
    std::cout << "Creating shader" << std::endl;
    int i;
    for (i = 0; i < 6; ++i) {
        if (sources[i]) {
            //prepare the shader
            *shaders[i] = glCreateShader(types[i]);
            glShaderSource(*shaders[i], 1, sources[i], nullptr);
            glCompileShader(*shaders[i]);
            GLint flag;
            glGetShaderiv(*shaders[i], GL_COMPILE_STATUS, &flag);
            if (!flag) {
                compiled = false;
                GLint logSize = 0;
                glGetShaderiv(*shaders[i], GL_INFO_LOG_LENGTH, &logSize);
                std::vector<GLchar> log(logSize);
                glGetShaderInfoLog(*shaders[i], logSize, nullptr, &log[0]);
                std::string fault;
                for (GLchar c: log) {
                    std::cout << c;
                    fault += c;
                }
                writeFile("./DEFAULT_" + std::to_string(i) + "_FAIL.txt", fault);
                break;
            }
        }
    }

    if (!compiled) {
        for (int j = 0; j < i; ++j) {
            if (sources[i]) {
                glDeleteShader(*shaders[i]);
                *shaders[i] = 0;
            }
        }
        return;
    }

    //todo Before linking setup
    program = glCreateProgram();
    for (unsigned int *shader: shaders) {
        if (*shader) {
            glAttachShader(program, *shader);
        }
    }

    glLinkProgram(program);

    //check link successful
    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint logSize = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

        std::vector<GLchar> infoLog(logSize);
        glGetProgramInfoLog(program, logSize, &logSize, &infoLog[0]);
        std::string fault;
        for (GLchar c: infoLog) {
            std::cout << c;
            fault += c;
        }
        writeFile("./DEFAULT_LINK_FAIL.txt", fault);

        glDeleteProgram(program); // also detaches shaders

        for (unsigned int *shader: shaders) {
            if (*shader) {
                glDeleteShader(*shader);
                *shader = 0;
            }
        }
        program = 0;
    }
}

GLuint Shader::getProgram() const {
    return program;
}
