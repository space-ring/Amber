//
// Created by croissant on 06/06/2022.
//

#include "AssetManager.h"
#include <iostream>
#include <fstream>

AssetManager::AssetManager() {

}

void AssetManager::addManifest(const string& manifest) {
    if (manifest.empty()) return;
    string line;
    std::ifstream openfile(manifest);
    size_t pos = 0;
    while (std::getline(openfile, line)) {
        string type, name;
        pos = line.find('@');
        type = line.substr(0, pos);
        line = line.substr(pos + 1);
        pos = line.find('@');
        name = line.substr(0, pos);
        line = line.substr(pos + 1);
        pos = line.find('@');

        if (type == "mesh") {
//            string path;
//            path = line.substr(pos + 1);
            auto pair = std::pair<string, string>(name, line);
            meshPaths->insert(pair);

//            engine->addMesh(name, loadMeshFile(path)->build());

        } else if (type == "shader") {
            string v, f, g, tc, te, c;
            string* stores[]{&v, &f, &g, &tc, &te, &c};

            bool exit;
            for (string* s: stores) {
                if (pos == string::npos) break;
                pos = line.find('@');
                *s = line.substr(0, pos);
                line = line.substr(pos + 1);
            }

            compoundShader paths{v, tc, te, g, f, c};
            shaderPaths->insert(std::pair<string, compoundShader>(name, paths));
//            engine->addShader(name, loadShaderFile(v, f, g, tc, te, c)->build());
        }
    }
    openfile.close();
}

void AssetManager::addShader(const string& name, const compoundShader& paths) {
    shaderPaths->insert(std::pair<string, compoundShader>(name, paths));
}

void AssetManager::addShader(const string& name, Shader& shader) {
    shaders->insert(std::pair<string, Shader*>(name, &shader));
}

void AssetManager::addMesh(const string& name, const string& path) {
    meshPaths->insert(std::pair<string, string>(name, path));
}

void AssetManager::addMesh(const string& name, Mesh& mesh) {
    meshes->insert(std::pair<string, Mesh*>(name, &mesh));
}

void AssetManager::addTexture(const string& name, const string& path) {
    texturePaths->insert(std::pair<string, string>(name, path));
}

void AssetManager::addTexture(const string& name, Texture& texture) {
    textures->insert(std::pair<string, Texture*>(name, &texture));
}

Shader* AssetManager::getShader(const string& name) {
    if (!shaders->contains(name)) {
        if (shaderPaths->contains(name)) {
            compoundShader paths = shaderPaths->at(name);
            addShader(name, *loadShaderFile(
                    paths.vertex,
                    paths.fragment,
                    paths.geometry,
                    paths.tessControl,
                    paths.tessEval,
                    paths.compute
            ));
        } else {
            return Shader::getDefault(); //todo don't insert default, manifest may be overridden at runtime (future feature)
        }
    }
    return shaders->at(name);
}

Mesh* AssetManager::getMesh(const string& name) {
    if (!meshes->contains(name)) {
        if (meshPaths->contains(name)) {
            addMesh(name, meshPaths->at(name));
        } else return Mesh::getDefault();
    }
    return meshes->at(name);
}

Texture* AssetManager::getTexture(const string& name) {
    if (!textures->contains(name)) {
        if (texturePaths->contains(name)) {
            addTexture(name, texturePaths->at(name));
        } else return nullptr; //todo texture
    }
    return textures->at(name);
}
