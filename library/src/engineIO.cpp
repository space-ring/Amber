//
// Created by croissant on 25/02/2022.
//

#include "engineIO.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "engineUtils.h"

//Assimp::Importer importer;

//todo throw / catch errors
namespace Amber {
    string readFile(const string& path) {
        if (path.empty()) return "";
        string file;
        string line;
        std::ifstream openfile(path);
        while (std::getline(openfile, line)) {
            file += line + "\n";
        }
        return file;
    }

    void writeFile(const string& path, const string& file) {
        std::ofstream openfile(path);
        openfile << file;
        openfile.close();
    }

    Shader* loadShaderFile(const compoundShader& paths) {
        return new Shader(
                new compoundShader{
                        readFile(paths.vertex),
                        readFile(paths.fragment),
                        readFile(paths.geometry),
                        readFile(paths.tessControl),
                        readFile(paths.tessEval),
                        readFile(paths.compute)
                }
        );
    }

    Shader* loadShader(const compoundShader& sources) {
        return new Shader(new compoundShader{
                sources.vertex, sources.fragment, sources.geometry,
                sources.tessControl, sources.tessEval, sources.compute
        });
    }

/*
Mesh* loadMeshFromScene(const aiScene* scene) {
    aiMesh* mesh = scene->mMeshes[0];
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;
        glm::vec3 vec;
        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z;
        vertex.position = vec;
        vec.x = mesh->mNormals[i].x;
        vec.y = mesh->mNormals[i].y;
        vec.z = mesh->mNormals[i].z;
        vertex.normal = vec;

        glm::vec2 tex(0.0f, 0.0f);
        if (mesh->mTextureCoords[0]) {
            tex.x = mesh->mTextureCoords[0][i].x;
            tex.y = mesh->mTextureCoords[0][i].y;
        }
        vertex.texUV = tex;
        vertices.push_back(vertex);
    }

    for (int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }
    return new Mesh(&vertices, &indices);
}
*/
    Mesh* loadMeshFile(const string& path) {
        return loadMesh(readFile(path));
    }

    Mesh* loadMesh(const string& mesh) {
        std::stringstream ss(mesh);
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
        return new Mesh(vertices, indices);
    }

    Texture* getTextureFile(const string& path, SupportedTextures type) {
        return 0;
    }

//Mesh* loadMeshFile(string& path) {
//    const aiScene* scene = importer.ReadFile(path, 0);
//    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//        std::cout << "Failed to load model from " << path << " " << importer.GetErrorString() << std::endl;
//    }
//    return loadMeshFromScene(scene);
//}

//Mesh* loadMesh(string& mesh) {
//    const aiScene* scene = importer.ReadFileFromMemory(mesh.c_str(), mesh.size(), 0);
//    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//        std::cout << "Failed to load model " << mesh << " " << importer.GetErrorString() << std::endl;
//    }
//    return loadMeshFromScene(scene);
//}

}