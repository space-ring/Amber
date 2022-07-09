//
// Created by croissant on 06/06/2022.
//

#include "AssetManager.h"
#include "engineIO.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "engineUtils.h"

AssetManager::AssetManager() {}

void AssetManager::buildAll() {
    Shader::getDefault()->build();
    for (const auto& shader: *shaderPaths) {
        addShader(shader.first, *loadShaderFile(shader.second)->build());
    }
    Mesh::getDefault()->build();
    for (const auto& mesh: *meshPaths) {
        addMesh(mesh.first, *loadMeshFile(mesh.second)->build());
    }
//    for (auto texture: *textures) {
//        texture.second->build();
//    }
}
//todo batch (name block instead of line)
//todo manifest includes (also check for circular includes)
void AssetManager::addManifest(const string& path) { //todo textures
    string manifest = readFile(path);
    if (manifest.empty()) return;
    string line;
    std::stringstream ss(manifest);
    while (std::getline(ss, line, '\n')) {
        auto items = split(line, '@');
        string type = items[0];
        string name = items[1];

        if (type == "mesh") {
            auto pair = std::pair<string, string>(name, items[2]);
            meshPaths->insert(pair);

        } else if (type == "shader") {
            string v, f, g, tc, te, c;
            string* stores[]{&v, &f, &g, &tc, &te, &c};

            for (int i = 0; i < items.size() - 2; ++i) {
                *stores[i] = items[i + 2];
            }

            compoundShader paths{v, f, g, tc, te, c};
            shaderPaths->insert(std::pair<string, compoundShader>(name, paths));
        }
    }
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
            addShader(name, *loadShaderFile(shaderPaths->at(name)));
        } else return Shader::getDefault();
    }
    return shaders->at(name);
}

Mesh* AssetManager::getMesh(const string& name) {
    if (!meshes->contains(name)) {
        if (meshPaths->contains(name)) {
            addMesh(name, *loadMeshFile(meshPaths->at(name)));
        } else return Mesh::getDefault();
    }
    return meshes->at(name);
}

Texture* AssetManager::getTexture(const string& name) {
    return nullptr;
    if (!textures->contains(name)) {
        if (texturePaths->contains(name)) {
            addTexture(name, texturePaths->at(name));
        } else return nullptr; //todo texture
    }
    return textures->at(name);
}

AssetManager::~AssetManager() {
    for (const auto& shader: *shaders) {
        delete shader.second;
    }
    delete shaders;
    delete Shader::getDefault();

    for (const auto& mesh: *meshes) {
        delete mesh.second;
    }
    delete meshes;
    delete Mesh::getDefault();
}
