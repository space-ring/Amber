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
		/**
		 * VAO Standard attributes:
		 * 0 = vertex positions
		 * 1 = vertex textures
		 * 2 = vertex normals
		 * 10 - 13 = instance transforms
		 * 14 - 17 = camera transforms
		 * 18 - 21 = projection transforms
		 */
		GLuint VAO = 0, vertexVBO = 0, EBO = 0;

	public:
		const unsigned int elementCount;

		explicit Mesh(const RawMesh& data);

		~Mesh();

		Mesh(const Mesh&) = delete;

		Mesh(Mesh&&) = delete;

		Mesh& operator=(const Mesh&) = delete;

		Mesh& operator=(Mesh&&) = delete;

		GLuint getVao() const;

		void linkInstanceTransforms(GLuint vbo);
	};
}


#endif //ENGINE_MESH_H
