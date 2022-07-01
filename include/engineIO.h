//
// Created by croissant on 25/02/2022.
//

#ifndef ENGINE_IO_H
#define ENGINE_IO_H

#include <string>
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
//#include "assimp/Importer.hpp"
//#include "assimp/scene.h"
//#include "assimp/postprocess.h"

//todo error detection & handling
//todo use string references etc

using string = std::string;

string readFile(const string &path);

void writeFile(const string &path, const string &file);

//todo put compute before tesselation
Shader *loadShaderFile(const compoundShader &paths);

Shader *loadShader(const compoundShader &sources);

/**
 * Constructs a mesh loaded from the given path
 * @param path path to mesh
 * @return Mesh object
 */
Mesh *loadMeshFile(const string &path);

/**
 * Constructs a mesh from the given source
 * @param mesh serialised mesh
 * @return Mesh object
 */
Mesh *loadMesh(const string &mesh);

Texture *getTextureFile(const string &path, enum SupportedTextures type); // todo texture types
#endif //ENGINE_IO_H