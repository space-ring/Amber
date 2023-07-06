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

	enum SupportedDataFormats {
		R = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		DEPTH = GL_DEPTH_COMPONENT,
		STENCIL = GL_DEPTH_STENCIL
	};

	enum SupportedImageFormats {
		R8 = GL_R8,
		RG8 = GL_RG8,
		RGB8 = GL_RGB8,
		RGBA8 = GL_RGBA8
	};

	struct RawTexture {
		SupportedTextures type;
		SupportedDataFormats format;
		unsigned char* data;
		int width, height, channels;

		explicit RawTexture(unsigned char* p, SupportedTextures type, SupportedDataFormats format, int width,
		                    int height,
		                    int channels)
				: type(type),
				  format(format),
				  width(width),
				  height(height),
				  channels(channels) {
			data = new unsigned char[width * height * channels]();
			if (p) memcpy(data, p, width * height * channels);
		}

		~RawTexture() {
			delete[] data;
		}
	};

	class Texture {
		SupportedTextures type;
		SupportedImageFormats internal;
		int width, height;
		unsigned int texture = 0;

	public:
		Texture(const RawTexture& data, SupportedImageFormats format, int levels = 3);

		~Texture();

		Texture(const Texture&) = delete;

		Texture(Texture&&) = delete;

		Texture& operator=(const Texture&) = delete;

		Texture& operator=(Texture&&) = delete;

		void bindToUnit(GLenum unit);

		void unbind();

		SupportedTextures getType() const;

		unsigned int getTexture() const;
	};

}

#endif //ENGINE_TEXTURE_H
