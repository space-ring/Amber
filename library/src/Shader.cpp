//
// Created by croissant on 01/04/2022.
//

#include <iostream>
#include <vector>
#include "Shader.h"
#include "engineIO.h"

namespace Amber {

	ShaderAttachment::ShaderAttachment(SupportedShaders type, GLuint program, view source)
			: program(program) {

		if (!source.empty()) {
			shader = glCreateShader(type);
			string str(source);
			const GLchar* const code = str.c_str();
			glShaderSource(shader, 1, &code, nullptr);
			glCompileShader(shader);

			GLint flag = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &flag);
			if (!flag) {
				std::cout << "Failed to compile shader " << type << std::endl << str << std::endl;
				GLint logSize = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
				std::vector<GLchar> log(logSize);
				glGetShaderInfoLog(shader, logSize, nullptr, &log[0]);
				// todo logging
				for (GLchar c: log) {
					std::cout << c;
				}
				glDeleteShader(shader);
				shader = 0;
				return;
			}

			if (program) glAttachShader(type, program);
		}
	}

	ShaderAttachment::~ShaderAttachment() {
		if (program && shader) glDetachShader(program, shader);
		if (shader) glDeleteShader(shader);
	}

	Shader::Shader(view srcVertex,
	               view srcTessControl,
	               view srcTessEval,
	               view srcGeometry,
	               view srcFragment,
	               view srcCompute) {

		program = glCreateProgram();

		ShaderAttachment vertex(VERTEX, program, srcVertex);
		ShaderAttachment tessControl(TESS_CONTROL, program, srcTessControl);
		ShaderAttachment tessEval(TESS_EVALUATION, program, srcTessEval);
		ShaderAttachment geometry(GEOMETRY, program, srcGeometry);
		ShaderAttachment fragment(FRAGMENT, program, srcFragment);
		ShaderAttachment compute(COMPUTE, program, srcCompute);

		// note pre-linkup here
		glLinkProgram(program);

		GLint flag = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &flag);
		if (!flag) {
			std::cout << "Failed to link shader program" << std::endl;
			GLint logSize = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
			std::vector<GLchar> log(logSize);
			glGetProgramInfoLog(program, logSize, &logSize, &log[0]);
			//todo logging
			for (GLchar c: log) {
				std::cout << c;
			}
			if (program) glDeleteProgram(program);
			program = 0;
		}
	}

	Shader::~Shader() {
		if (program) glDeleteProgram(program);
	}

	void Shader::start() {
		if (program) glUseProgram(program);
	}

	void Shader::stop() {
		glUseProgram(0);
	}

}