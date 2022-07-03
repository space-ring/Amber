//
// Created by croissant on 01/04/2022.
//

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "graphics.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texUV;
};

class Mesh { //todo destroy
private:
    static Mesh* DEFAULT;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    /**
     * vao Standard attributes:
     * 0 = vertex positions
     * 1 = vertex textures
     * 2 = vertex normals
     * 10 - 13 = instance transforms
     * 14 - 17 = camera transforms
     * 18 - 21 = perspective transforms
     */
    GLuint vao{0}, vbo{0}, ebo{0};

public:
    static Mesh* getDefault();

    Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices);

    virtual ~Mesh();

    Mesh* build();
};


#endif //ENGINE_MESH_H
