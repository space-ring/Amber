//
// Created by croissant on 14/06/2023.
//

#ifndef ENGINETEST_FRAMEBUFFER_H
#define ENGINETEST_FRAMEBUFFER_H

#include "graphics.h"
#include "Texture.h"

namespace Amber {

	enum class SupportedFBOAttachments {
		COLOUR = GL_COLOR_ATTACHMENT0,
		DEPTH = GL_DEPTH_ATTACHMENT,
		STENCIL = GL_STENCIL_ATTACHMENT,
		DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
	};

	class Frame {
		unsigned int fbo;

	public:
		Frame();

		~Frame();

		void attachDraw(SupportedFBOAttachments point, Texture& texture);

		void attachRead(SupportedFBOAttachments point, Texture& texture);
	};
}

#endif //ENGINETEST_FRAMEBUFFER_H
