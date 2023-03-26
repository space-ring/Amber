//
// Created by croissant on 01/04/2022.
//

#ifndef ENGINE_SHADER_H
#define ENGINE_SHADER_H

#include <string>
#include "graphics.h"

/* Shader takes individual shader code for each of SupportedShaders.
 * Attachment compiles the code and attaches to program. Detach and delete on dtor.
 */

namespace Amber {
	enum SupportedShaders {
		VERTEX = GL_VERTEX_SHADER,
		TESS_CONTROL = GL_TESS_CONTROL_SHADER,
		TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		COMPUTE = GL_COMPUTE_SHADER
	};

	struct ShaderAttachment {

		using string = std::string;
		using view = std::string_view;

		GLuint program, shader;

		ShaderAttachment(SupportedShaders type, GLuint program, view source);

		~ShaderAttachment();

		ShaderAttachment(const ShaderAttachment&) = delete;

		ShaderAttachment& operator=(const ShaderAttachment&) = delete;
	};

	class Shader {

		using string = std::string;
		using view = std::string_view;

		GLuint program;

	public:
		Shader(view srcVertex, view srcTessControl, view srcTessEval, view srcGeometry, view srcFragment,
		       view srcCompute);

		~Shader();

		Shader(const Shader&) = delete;

		Shader(Shader&&) = delete;

		Shader& operator=(const Shader&) = delete;

		Shader& operator=(Shader&&) = delete;

		void start();

		void stop();
	};

}

#endif //ENGINE_SHADER_H
