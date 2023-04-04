//
// Created by croissant on 01/04/2022.
//

#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

#include "graphics.h"

namespace Amber {
	enum SupportedTextures { //todo add more texture types
		TEXTURE_2D = GL_TEXTURE_2D
	};

	struct RawTexture {
		SupportedTextures type;
		unsigned char* data;
		int width, height, channels;

		explicit RawTexture(unsigned char* p, SupportedTextures type, int width, int height, int channels)
				: type(type),
				  width(width),
				  height(height),
				  channels(channels) {
			memcpy(data, p, width * height * channels);
		}
	};

	class Texture {
		SupportedTextures type;
		int width, height, channels;
		unsigned int texture;

	public:
		Texture(const RawTexture& data);

		~Texture();

		Texture(const Texture&) = delete;

		Texture(Texture&&) = delete;

		Texture& operator=(const Texture&) = delete;

		Texture& operator=(Texture&&) = delete;


	};

}

#endif //ENGINE_TEXTURE_H
