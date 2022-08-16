//
// Created by croissant on 06/06/2022.
//

#include "AssetManager.h"
#include "engineIO.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "engineUtils.h"

namespace Amber {
	AssetManager::AssetManager() {}

	/*
	 * The problem here was that if an asset was retrieved before calling this method,
	 * freshly constructed and built meshes could not be added since there would've already been a mapping.
	 * So split the calls into 2, one for setting the mapping (if it doesn't already exist) and then build.
	 */
	void AssetManager::buildAll() {
		Shader::getDefault()->build();
		for (const auto& shader: *shaderPaths) {
			addShader(shader.first, *loadShaderFile(shader.second));
			getShader(shader.first)->build();
		}
		Mesh::getDefault()->build();
		for (const auto& mesh: *meshPaths) {
			addMesh(mesh.first, *loadMeshFile(mesh.second)); //add if not constructed
			getMesh(mesh.first)->build(); //and build (whether previously retrieved or not)
		}
//    for (auto texture: *textures) {
//        texture.renderState->build();
//    }
	}

//todo batch (name block instead of line)
//todo manifest includes (also check for circular includes) not high priority
//todo component paths e.g. all meshes in assets/meshes
//todo the whole manifest language!!! default to json maybe...
	void AssetManager::addManifest(const string& path) { //todo textures
		string manifest = readFile(path);
		if (manifest.empty()) return;
		string line;
		std::stringstream ss(manifest);
		while (std::getline(ss, line, '\n')) {
			if (line.starts_with("#") || line.empty()) continue;
			auto items = split(line, ' ');
			string type = items[0];
			string name = items[1];

			if (type == "mesh") {
				addMesh(name, items[2]);

			} else if (type == "shader") {
				string v, f, g, tc, te, c;
				string* stores[]{&v, &f, &g, &tc, &te, &c};

				for (int i = 0; i < items.size() - 2; ++i) {
					*stores[i] = items[i + 2];
				}

				compoundShader paths{v, f, g, tc, te, c};
				addShader(name, paths);
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
		int x = 0;
		for (const auto& shader: *shaders) {
			delete shader.second;
		}
		delete shaders;

		for (const auto& mesh: *meshes) {
			delete mesh.second;
		}
		delete meshes;
	}

}