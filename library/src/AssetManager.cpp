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

	RawMesh parseMeshOBJ(view mesh) {
		string str(mesh);
		std::stringstream ss(str);
		string line;
		std::vector<glm::vec3> positions, normals;
		std::vector<glm::vec2> uv;
		std::map<string, unsigned int> seen; // indices to vertices vector
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int index = 0;

		while (std::getline(ss, line, '\n')) {
			if (line.empty()) continue;
			auto spaced_line = split(line, ' ');
			if (spaced_line[0] == "v") {
				glm::vec3 vec3;
				vec3.x = std::stof(spaced_line[1]);
				vec3.y = std::stof(spaced_line[2]);
				vec3.z = std::stof(spaced_line[3]);
				positions.push_back(vec3);

			} else if (spaced_line[0] == "vt") {
				glm::vec2 vec2;
				vec2.x = std::stof(spaced_line[1]);
				vec2.y = std::stof(spaced_line[2]);
				uv.push_back(vec2);

			} else if (spaced_line[0] == "vn") {
				glm::vec3 vec3;
				vec3.x = std::stof(spaced_line[1]);
				vec3.y = std::stof(spaced_line[2]);
				vec3.z = std::stof(spaced_line[3]);
				normals.push_back(vec3);

			} else if (spaced_line[0] == "f") {
				for (int i = 1; i < 4; ++i) {
					if (seen.contains(spaced_line[i])) {
						indices.push_back(seen[spaced_line[i]]);
					} else {
						Vertex vertex;
						auto iface = isplit(spaced_line[i], '/');
						vertex.position = positions[iface[0] - 1];
						if (iface.size() == 2) {
							vertex.normal = normals[iface[1] - 1];
							vertex.texUV = glm::vec2(0, 0);
						} else {
							vertex.texUV = uv[iface[1] - 1];
							vertex.normal = normals[iface[2] - 1];
						}
						vertices.push_back(vertex);
						seen.insert(std::pair<string, unsigned int>(spaced_line[i], index));
						indices.push_back(index++);
					}
				}
			}
		}
		return {vertices, indices};
	}

	AssetManager::AssetManager() {
		//todo insert defaults
	}

	void AssetManager::addManifest(view path) {
		string manifest = readFile(path);

		if (manifest.empty()) return;
		string line;
		std::stringstream ss(manifest);
		while (std::getline(ss, line, '\n')) {
			if (line.starts_with("#") || line.empty()) continue;
			auto items = split(line, ' ');
			string type = items[0];
			token id = std::stoull(items[1]);

			if (type == "shader") {
				addShaderPath(id, items[2]);
			} else if (type == "program") {

			}

			//todo mesh, texture.
		}
	}

	void AssetManager::addShaderPath(token id, view path) {
		shaderPaths.emplace(id, path);
	}

	void AssetManager::addMeshPath(token id, view path) {
		meshPaths.emplace(id, path);
	}

	void AssetManager::addTexturePath(token id, view path) {
		texturePaths.emplace(id, path);
	}

	view AssetManager::getRawShader(token id, bool load) {
		if (!rawShaders.contains(id)) {
			if (load)
				return loadRawShader(id);
			else
				return rawShaders.at(0);
		}
		return rawShaders.at(id);
	}

	RawMesh& AssetManager::getRawMesh(token id, bool load) {
		if (!rawMeshes.contains(id)) {
			if (load)
				return loadRawMesh(id);
			else
				return rawMeshes.at(0);
		}
		return rawMeshes.at(id);
	}

	RawTexture& AssetManager::getRawTexture(token id, bool load) {
		if (!rawTextures.contains(id)) {
			if (load)
				return loadRawTexture(id);
			else
				return rawTextures.at(0);
		}
		return rawTextures.at(id);
	}

	view AssetManager::loadRawShader(token id) {
		if (!shaderPaths.contains(id)) return rawShaders.at(0);
		rawShaders.emplace(id, readFile(shaderPaths.at(id)));
		return rawShaders.at(id);
	}

	RawMesh& AssetManager::loadRawMesh(token id) {
		if (!shaderPaths.contains(id)) return rawMeshes.at(0);
		rawMeshes.emplace(id, parseMeshOBJ(readFile(meshPaths.at(id))));
		return rawMeshes.at(id);
	}

	RawTexture& AssetManager::loadRawTexture(token id) {
		throw 500;
	}

	void AssetManager::unloadRawShader(token id) {
		rawShaders.erase(id);
	}

	void AssetManager::unloadRawMesh(token id) {
		rawMeshes.erase(id);
	}

	void AssetManager::unloadRawTexture(token id) {
		rawTextures.erase(id);
	}

	Shader& AssetManager::createShader(token id,
	                                   const list<token>& vertex,
	                                   const list<token>& tessControl,
	                                   const list<token>& tessEval,
	                                   const list<token>& geometry,
	                                   const list<token>& fragment,
	                                   const list<token>& compute) {

		//todo currently every rawshader has to be recompiled
		int i = 0;
		const char** vsource = new const char* [vertex.size()];
		int* vlength = new int[vertex.size()];
		for (auto t: vertex) {
			vsource[i++] = rawShaders[t].c_str();
		}
		ShaderStitch vStitch{static_cast<int>(vertex.size()), vsource, vlength};


		i = 0;
		const char** tcsource = new const char* [vertex.size()];
		int* tclength = new int[vertex.size()];
		for (auto t: vertex) {
			vsource[i++] = rawShaders[t].c_str();
		}
		ShaderStitch tcStitch{static_cast<int>(vertex.size()), vsource, tclength};

		i = 0;
		const char** tesource = new const char* [vertex.size()];
		int* telength = new int[vertex.size()];
		for (auto t: vertex) {
			vsource[i++] = rawShaders[t].c_str();
		}
		ShaderStitch teStitch{static_cast<int>(vertex.size()), vsource, telength};

		i = 0;
		const char** gsource = new const char* [vertex.size()];
		int* glength = new int[vertex.size()];
		for (auto t: vertex) {
			vsource[i++] = rawShaders[t].c_str();
		}
		ShaderStitch gStitch{static_cast<int>(vertex.size()), gsource, glength};

		i = 0;
		const char** fsource = new const char* [vertex.size()];
		int* flength = new int[vertex.size()];
		for (auto t: vertex) {
			vsource[i++] = rawShaders[t].c_str();
		}
		ShaderStitch fStitch{static_cast<int>(vertex.size()), fsource, flength};

		i = 0;
		const char** csource = new const char* [vertex.size()];
		int* clength = new int[vertex.size()];
		for (auto t: vertex) {
			vsource[i++] = rawShaders[t].c_str();
		}
		ShaderStitch cStitch{static_cast<int>(vertex.size()), csource, clength};


		shaders.emplace(std::piecewise_construct, std::make_tuple(id),
		                std::make_tuple(vStitch, tcStitch, teStitch, gStitch, fStitch, cStitch));

		delete[] vsource;
		delete[] tcsource;
		delete[] tesource;
		delete[] gsource;
		delete[] fsource;
		delete[] csource;
		delete[] vlength;
		delete[] tclength;
		delete[] telength;
		delete[] glength;
		delete[] flength;
		delete[] clength;

		return shaders.at(id);
	}

	Shader& AssetManager::getShader(token id) {
		if (shaders.contains(id)) return shaders.at(id);
		return shaders.at(0);
	}

	Mesh& AssetManager::getMesh(token id) {
		if (meshes.contains(id)) return meshes.at(id);
		return meshes.at(0);
	}

	Texture& AssetManager::getTexture(token id) {
		if (textures.contains(id)) return textures.at(id);
		return textures.at(0);
	}

	AssetManager::~AssetManager() {

	}

}