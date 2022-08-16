//
// Created by croissant on 16/08/2022.
//

#include "Layer.h"

namespace Amber {

	Model* Layer::pick(int x, int y) {
		glClearColor(0, 0, 0, 0);
		glClearDepth(1);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		return nullptr;
	}
}
