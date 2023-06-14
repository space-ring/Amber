//
// Created by croissant on 14/06/2023.
//

#include "Framebuffer.h"

namespace Amber {

	Framebuffer::Framebuffer() {
		glGenFramebuffers(1, &fbo);
	}

	Framebuffer::~Framebuffer() {
		glDeleteFramebuffers(1, &fbo);
	}

	void Framebuffer::attachDraw(SupportedFBOAttachments point, Texture& texture) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		switch (texture.getType()) {
			case TEXTURE_2D:
				glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, static_cast<GLenum>(point), texture.getType(),
				                       texture.getTexture(), 0);
				break;
		}
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void Framebuffer::attachRead(SupportedFBOAttachments point, Texture& texture) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		switch (texture.getType()) {
			case TEXTURE_2D:
				glFramebufferTexture2D(GL_READ_FRAMEBUFFER, static_cast<GLenum>(point), texture.getType(),
				                       texture.getTexture(), 0);
				break;
		}
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

}