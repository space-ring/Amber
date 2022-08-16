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
			"//basic.vert\n"
			"#version 430 core\n"
			"\n"
			"layout (location = 0) in vec3 position;\n"
			"layout (location = 1) in vec2 uv;\n"
			"layout (location = 2) in vec3 normal;\n"
			"layout (location = 10) in mat4 M;\n"
			"layout (location = 14) uniform mat4 V;\n"
			"layout (location = 18) uniform mat4 P;\n"
			"\n"
			"out vec3 frag_position;\n"
			"\n"
			"void main(void){\n"
			"    frag_position = position;\n"
			"    gl_Position = P * V * M * vec4(position, 1.0);\n"
			"}";


	string DEFAULT_FRAGMENT =
			"//basic.frag\n"
			"#version 430 core\n"
			"\n"
			"in vec3 frag_position;\n"
			"out vec4 fragment;\n"
			"\n"
			"void main(void){\n"
			"    fragment = vec4(frag_position/2+0.5, 1.0);\n"
			"}";


	Shader* Shader::getDefault() { //todo upcast for GL context (what??)
		static Shader DEFAULT({DEFAULT_VERTEX, DEFAULT_FRAGMENT});
		return &DEFAULT;
	}

	Shader::Shader(GLuint program, GLuint vertex, GLuint tessCtrl, GLuint tessEval, GLuint geometry, GLuint fragment,
	               GLuint compute)
			: program(program), vertex(vertex), tessCtrl(tessCtrl),
			  tessEval(tessEval), geometry(geometry), fragment(fragment), compute(compute) {
	}

	Shader::Shader(const compoundShader& sources) : sources(sources) {}

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
	}

	Shader* Shader::start() {
		//todo if !program throw error
		glUseProgram(program);
		return this;
	}

	Shader* Shader::stop() {
		glUseProgram(0);
		return this;
	}

	//todo delete previous shader if assigned
	//todo this doesn't allow multiple of the same type (as gl spec allows)
	GLuint Shader::addShader(SupportedShaders type, const GLchar* const* code) {
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, code, nullptr);
		glCompileShader(shader);
		GLint flag = 0;
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

		glCheckError();
		GLuint p = glCreateProgram();
		glCheckError();
		if (!sources.vertex.empty()) {
			const char* str = sources.vertex.c_str();
			addShader(VERTEX, &str);
			glAttachShader(p, vertex);
			glCheckError();
		}
		if (!sources.tessControl.empty()) {
			const char* str = sources.tessControl.c_str();
			addShader(TESS_CONTROL, &str);
			glAttachShader(p, tessCtrl);
			glCheckError();
		}
		if (!sources.tessEval.empty()) {
			const char* str = sources.tessEval.c_str();
			addShader(TESS_EVALUATION, &str);
			glAttachShader(p, tessEval);
			glCheckError();
		}
		if (!sources.geometry.empty()) {
			const char* str = sources.geometry.c_str();
			addShader(GEOMETRY, &str);
			glAttachShader(p, geometry);
			glCheckError();
		}
		if (!sources.fragment.empty()) {
			const char* str = sources.fragment.c_str();
			addShader(FRAGMENT, &str);
			glAttachShader(p, fragment);
			glCheckError();
		}
		if (!sources.compute.empty()) {
			const char* str = sources.compute.c_str();
			addShader(COMPUTE, &str);
			glAttachShader(p, compute);
			glCheckError();
		}

		glCheckError();
		//todo pre-linkup build e.g. transform feedback
		glLinkProgram(p);
		glCheckError();
		GLint flag = 0;
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
		glCheckError();
		program = p;
		return this;
	}

}