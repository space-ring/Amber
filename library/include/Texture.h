//
// Created by croissant on 01/04/2022.
//

#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

#include "graphics.h"
#include "Shader.h"

namespace Amber {
	enum SupportedTextures { //todo add more texture types
		TEXTURE_2D = GL_TEXTURE_2D
	};

	class Texture {
		static Texture* DEFAULT;

		unsigned char* texels;

		GLuint texture = 0;

	public:
		static Texture* getDefault();

		static void deleteDefault();

		Texture(unsigned char* data);

		Texture* build();
	};

}

#endif //ENGINE_TEXTURE_H
