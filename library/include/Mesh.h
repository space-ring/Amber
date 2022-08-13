//
// Created by croissant on 01/04/2022.
//

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "graphics.h"

namespace Amber {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 texUV;
        glm::vec3 normal;
    };

    struct RawMesh {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };

    class Mesh {
    private:

        RawMesh data;

        /**
         * VAO Standard attributes:
         * 0 = vertex positions
         * 1 = vertex textures
         * 2 = vertex normals
         * 3 = model pick colour
         * 10 - 13 = instance transforms
         * 14 - 17 = camera transforms
         * 18 - 21 = projection transforms
         */
        GLuint VAO{0},
                vertexVBO{0},
                instanceVBO{0},
                EBO{0};

    public:
        static Mesh* getDefault();

        Mesh(const RawMesh& data);

        virtual ~Mesh();

        Mesh* build();

        GLuint getInstanceVbo() const;

        GLuint getVao() const;

        GLuint getElementCount() const;

        GLuint getEbo() const;
    };
}


#endif //ENGINE_MESH_H
