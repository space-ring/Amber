//
// Created by croissant on 01/04/2022.
//

#include <iostream>
#include <vector>
#include "Shader.h"

namespace Amber {

	Shader::ShaderAttachment::ShaderAttachment(SupportedShaders type, GLuint program, ShaderStitch source)
			: program(program) {

		if (!source.count) return;

		shader = glCreateShader(type);
		glShaderSource(shader, source.count, source.string, source.lengths);
		glCompileShader(shader);

		GLint flag = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &flag);
		if (!flag) {
			std::cout << "Failed to compile shader: " << type << std::endl;
			for (int i = 0; i < source.count; ++i) {
				for (int j = 0; j < source.lengths[i]; ++j) {
					std::cout << source.string[i][j];
				}
			}
			std::cout << std::endl;
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

		if (program) glAttachShader(program, shader);
	}

	Shader::ShaderAttachment::~ShaderAttachment() {
		if (program && shader) glDetachShader(program, shader);
		if (shader) glDeleteShader(shader);
	}

	Shader::Shader(ShaderStitch srcVertex,
	               ShaderStitch srcTessControl,
	               ShaderStitch srcTessEval,
	               ShaderStitch srcGeometry,
	               ShaderStitch srcFragment,
	               ShaderStitch srcCompute) {

		program = glCreateProgram();

		//todo move compilation, linking, attaching and detaching into functions?

		{
			ShaderAttachment vertex(VERTEX, program, srcVertex);
			ShaderAttachment tessControl(TESS_CONTROL, program, srcTessControl);
			ShaderAttachment tessEval(TESS_EVALUATION, program, srcTessEval);
			ShaderAttachment geometry(GEOMETRY, program, srcGeometry);
			ShaderAttachment fragment(FRAGMENT, program, srcFragment);
			ShaderAttachment compute(COMPUTE, program, srcCompute);

			// note pre-linkup here
			glLinkProgram(program);
		}

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

	Shader& Shader::start() {
		if (program) glUseProgram(program);
		return *this;
	}

	Shader& Shader::stop() {
		glUseProgram(0);
		return *this;
	}

}