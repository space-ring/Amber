//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_IO_H
#define ENGINE_IO_H

#include <string>

//todo error detection & handling
namespace Amber {
	using string = std::string;
	using view = std::string_view;

	string readFile(view path);

	void writeFile(view path, view file);

}
#endif //ENGINE_IO_H