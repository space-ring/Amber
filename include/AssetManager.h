//
// Created by croissant on 06/06/2022.
//

#ifndef ENGINE_ASSETMANAGER_H
#define ENGINE_ASSETMANAGER_H

#include <map>
#include <string>
#include "Mesh.h"
#include "Texture.h"
#include "engineIO.h"

class AssetManager {
    using string = std::string;

private: //todo const maps?
    std::map<string, compoundShader>* shaderPaths = new std::map<string, compoundShader>;
    std::map<string, string>* meshPaths = new std::map<string, string>;
    std::map<string, string>* texturePaths = new std::map<string, string>;
    std::map<string, Shader*>* shaders = new std::map<string, Shader*>;
    std::map<string, Mesh*>* meshes = new std::map<string, Mesh*>;
    std::map<string, Texture*>* textures = new std::map<string, Texture*>;

public:
    AssetManager();

    void addManifest(const string& manifest);

    void addShader(const string& name, const compoundShader& paths);

    void addShader(const string& name, Shader& shader);

    void addMesh(const string& name, const string& path);

    void addMesh(const string& name, Mesh& mesh);

    void addTexture(const string& name, const string& path);

    void addTexture(const string& name, Texture& texture);

    Shader* getShader(const string& name);

    Mesh* getMesh(const string& name);

    Texture* getTexture(const string& name);
};


#endif //ENGINE_ASSETMANAGER_H
