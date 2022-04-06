//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_IO_H
#define ENGINE_IO_H

#include <string>
#include "Shader.h"
#include "Texture.h"

//todo error detection & handling

using string = std::string;

string readFile(const string &path);

void writeFile(const string &path, string file);

Shader *getShader(string vertex, string tessControl, string tessEval, string geometry, string fragment, string compute);

Shader *getShader(string vertex, string geometry, string fragment);

Shader *getShader(string vertex, string fragment);

Texture *getTexture(string path); // todo texture types

#endif //ENGINE_IO_H