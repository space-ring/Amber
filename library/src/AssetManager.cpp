//
// Created by croissant on 06/06/2022.
//

#include "AssetManager.h"
#include "engineIO.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "engineUtils.h"

int strToShaderType(std::string_view str) {
	if (str == "V") return 0;
	if (str == "TC") return 1;
	if (str == "TE") return 2;
	if (str == "G") return 3;
	if (str == "F") return 4;
	if (str == "C") return 5;
	throw std::runtime_error("Unknown shader type " + std::string(str));
}

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
						Vertex vertex{};
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

	void AssetManager::addManifest(view path) {
		string manifest = readFile(path);

		if (manifest.empty()) return;
		string line;
		string asset;
		std::stringstream ss(manifest);
		while (std::getline(ss, line, '\n')) {
			if (line.starts_with("#") || line.empty()) continue;
			auto items = split(line, ' ');
			if (items.size() == 1) {
				asset = items[0];
				continue;
			}

			token id = std::stoull(items[0]);

			if (asset == "shader") {
				addSourcePath(id, items[1]);

			} else if (asset == "program") {

				list<token> refs[6];
				// note this conversion assumes order of shaders
				int type = strToShaderType(items[1]);
				for (int i = 2; i < items.size(); ++i) {
					if (items[i][0] < 58)
						refs[type].push_back(std::stoull(items[i]));
					else {
						type = strToShaderType(items[i]);
					}
				}
				addShaderFormula(id, {refs[0], refs[1], refs[2], refs[3], refs[4], refs[5]});

			} else if (asset == "mesh") {
				addMeshPath(id, items[1]);
			}

			//todo texture
		}
	}

	void AssetManager::addSourcePath(token id, view path) {
		sourcePaths.emplace(id, path);
	}

	view AssetManager::loadSource(token id) {
		if (!sourcePaths.contains(id))
			throw std::runtime_error("Cannot load unknown shader source " + std::to_string(id));
		sources.emplace(id, readFile(sourcePaths.at(id)));
		return sources.at(id);
	}

	void AssetManager::unloadSource(token id) {
		sources.erase(id);
	}

	view AssetManager::getSource(token id) {
		if (sources.contains(id)) return sources.at(id);
		return loadSource(id);
	}

	void AssetManager::addShaderFormula(AssetManager::token id, const AssetManager::ShaderFormula& formula) {
		shaderFormulas.emplace(id, formula);
	}

	Shader& AssetManager::loadShader(token id) {

		if (!shaderFormulas.contains(id))
			throw std::runtime_error("Cannot create unknown shader " + std::to_string(id));

		auto vertex = shaderFormulas.at(id).V;
		auto tessControl = shaderFormulas.at(id).TC;
		auto tessEval = shaderFormulas.at(id).TE;
		auto geometry = shaderFormulas.at(id).G;
		auto fragment = shaderFormulas.at(id).F;
		auto compute = shaderFormulas.at(id).C;

		//todo currently every rawshader has to be recompiled
		int i = 0;
		const char** vsource = new const char* [vertex.size()];
		int* vlength = new int[vertex.size()];
		for (auto t: vertex) {
			vlength[i] = getSource(t).size();
			vsource[i++] = getSource(t).data();
		}
		ShaderStitch vStitch{static_cast<int>(vertex.size()), vsource, vlength};


		i = 0;
		const char** tcsource = new const char* [tessControl.size()];
		int* tclength = new int[tessControl.size()];
		for (auto t: tessControl) {
			tclength[i] = getSource(t).size();
			tcsource[i++] = getSource(t).data();
		}
		ShaderStitch tcStitch{static_cast<int>(tessControl.size()), tcsource, tclength};

		i = 0;
		const char** tesource = new const char* [tessEval.size()];
		int* telength = new int[tessEval.size()];
		for (auto t: tessEval) {
			telength[i] = getSource(t).size();
			tesource[i++] = getSource(t).data();
		}
		ShaderStitch teStitch{static_cast<int>(tessEval.size()), tesource, telength};

		i = 0;
		const char** gsource = new const char* [geometry.size()];
		int* glength = new int[geometry.size()];
		for (auto t: geometry) {
			glength[i] = getSource(t).size();
			gsource[i++] = getSource(t).data();
		}
		ShaderStitch gStitch{static_cast<int>(geometry.size()), gsource, glength};

		i = 0;
		const char** fsource = new const char* [fragment.size()];
		int* flength = new int[fragment.size()];
		for (auto t: fragment) {
			flength[i] = getSource(t).size();
			fsource[i++] = getSource(t).data();
		}
		ShaderStitch fStitch{static_cast<int>(fragment.size()), fsource, flength};

		i = 0;
		const char** csource = new const char* [compute.size()];
		int* clength = new int[compute.size()];
		for (auto t: compute) {
			clength[i] = getSource(t).size();
			csource[i++] = getSource(t).data();
		}
		ShaderStitch cStitch{static_cast<int>(compute.size()), csource, clength};


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
		return loadShader(id);
	}

	void AssetManager::addMeshPath(token id, view path) {
		meshPaths.emplace(id, path);
	}

	RawMesh& AssetManager::loadRawMesh(token id) {
		if (!meshPaths.contains(id))
			throw std::runtime_error("Cannot load unknown mesh data " + std::to_string(id));
		rawMeshes.emplace(id, parseMeshOBJ(readFile(meshPaths.at(id))));
		return rawMeshes.at(id);
	}

	void AssetManager::unloadRawMesh(token id) {
		rawMeshes.erase(id);
	}

	RawMesh& AssetManager::getRawMesh(token id) {
		if (rawMeshes.contains(id)) return rawMeshes.at(id);
		return loadRawMesh(id);
	}

	Mesh& AssetManager::getMesh(token id) {
		if (meshes.contains(id)) return meshes.at(id);
		return meshes.at(0);
	}

}