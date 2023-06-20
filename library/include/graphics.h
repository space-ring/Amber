//
// Created by croissant on 01/04/2022.
//

#ifndef ENGINE_GRAPHICS_H
#define ENGINE_GRAPHICS_H

//#define GLFW_DLL

#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

GLenum glCheckError_(const char* file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__)

#endif //ENGINE_GRAPHICS_H
