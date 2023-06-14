//
// Created by croissant on 01/04/2022.
//

#include "Texture.h"

namespace Amber {
	Texture::Texture(const RawTexture& data, SupportedInternalFormats internal) :
			type(data.type),
			format(data.format),
			internal(internal),
			width(data.width),
			height(data.height) {

		glGenTextures(1, &texture);
		glBindTexture(type, texture);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		switch (type) {
			case TEXTURE_2D:
				glTexImage2D(type, 0, internal, width, height, 0, format, GL_UNSIGNED_BYTE, data.data);
				break;
		}

		glGenerateMipmap(type);
		glBindTexture(type, 0);
	}

	Texture::~Texture() {
		if (texture) glDeleteTextures(1, &texture);
	}

	void Texture::bindToAt(GLenum unit) {
		glActiveTexture(unit);
		glBindTexture(type, texture);
	}

	void Texture::unbind() {
		glBindTexture(type, 0);
	}

	SupportedTextures Texture::getType() const {
		return type;
	}

	unsigned int Texture::getTexture() const {
		return texture;
	}

}
