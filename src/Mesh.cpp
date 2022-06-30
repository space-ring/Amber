//
// Created by croissant on 01/04/2022.
//

#include "Mesh.h"
#include "engineIO.h"

std::string DEFAULT_MESH =
        "# Blender v3.1.2 OBJ File: ''\n"
        "# www.blender.org\n"
        "o Cube\n"
        "v 1.000000 1.000000 -1.000000\n"
        "v 1.000000 -1.000000 -1.000000\n"
        "v 1.000000 1.000000 1.000000\n"
        "v 1.000000 -1.000000 1.000000\n"
        "v -1.000000 1.000000 -1.000000\n"
        "v -1.000000 -1.000000 -1.000000\n"
        "v -1.000000 1.000000 1.000000\n"
        "v -1.000000 -1.000000 1.000000\n"
        "vt 0.625000 0.500000\n"
        "vt 0.875000 0.500000\n"
        "vt 0.875000 0.750000\n"
        "vt 0.625000 0.750000\n"
        "vt 0.375000 0.750000\n"
        "vt 0.625000 1.000000\n"
        "vt 0.375000 1.000000\n"
        "vt 0.375000 0.000000\n"
        "vt 0.625000 0.000000\n"
        "vt 0.625000 0.250000\n"
        "vt 0.375000 0.250000\n"
        "vt 0.125000 0.500000\n"
        "vt 0.375000 0.500000\n"
        "vt 0.125000 0.750000\n"
        "vn 0.0000 1.0000 0.0000\n"
        "vn 0.0000 0.0000 1.0000\n"
        "vn -1.0000 0.0000 0.0000\n"
        "vn 0.0000 -1.0000 0.0000\n"
        "vn 1.0000 0.0000 0.0000\n"
        "vn 0.0000 0.0000 -1.0000\n"
        "s off\n"
        "f 1/1/1 5/2/1 7/3/1 3/4/1\n"
        "f 4/5/2 3/4/2 7/6/2 8/7/2\n"
        "f 8/8/3 7/9/3 5/10/3 6/11/3\n"
        "f 6/12/4 2/13/4 4/5/4 8/14/4\n"
        "f 2/13/5 1/1/5 3/4/5 4/5/5\n"
        "f 6/11/6 5/10/6 1/1/6 2/13/6";

Mesh* Mesh::DEFAULT = nullptr;

Mesh::Mesh(std::vector<Vertex>* vertices, std::vector<unsigned int>* indices)
        : vertices(*vertices),
          indices(*indices) {
}

Mesh* Mesh::getDefault() {
    if (!DEFAULT) {
        DEFAULT = loadMesh(DEFAULT_MESH);
    }
    return DEFAULT;
}

Mesh* Mesh::build() {
    if (vao) return this; //build once

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) nullptr);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texUV));
    //todo other attributes (see .h)

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return this;
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

