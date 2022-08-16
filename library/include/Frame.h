//
// Created by croissant on 28/05/2022.
//

#ifndef ENGINE_FRAME_H
#define ENGINE_FRAME_H
namespace Amber {
	struct Frame {
		unsigned int x, y, width, height;

		Frame(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	};

}
#endif //ENGINE_FRAME_H
