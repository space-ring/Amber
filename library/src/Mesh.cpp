//
// Created by croissant on 01/04/2022.
//

#include "Mesh.h"
#include "engineIO.h"

namespace Amber {
	static std::string DEFAULT_MESH =
			"o Cube\n"
			"v 1.000000 1.000000 -1.000000\n"
			"v 1.000000 -1.000000 -1.000000\n"
			"v 1.000000 1.000000 1.000000\n"
			"v 1.000000 -1.000000 1.000000\n"
			"v -1.000000 1.000000 -1.000000\n"
			"v -1.000000 -1.000000 -1.000000\n"
			"v -1.000000 1.000000 1.000000\n"
			"v -1.000000 -1.000000 1.000000\n"
			"vt 0.875000 0.500000\n"
			"vt 0.625000 0.750000\n"
			"vt 0.625000 0.500000\n"
			"vt 0.375000 1.000000\n"
			"vt 0.375000 0.750000\n"
			"vt 0.625000 0.000000\n"
			"vt 0.375000 0.250000\n"
			"vt 0.375000 0.000000\n"
			"vt 0.375000 0.500000\n"
			"vt 0.125000 0.750000\n"
			"vt 0.125000 0.500000\n"
			"vt 0.625000 0.250000\n"
			"vt 0.875000 0.750000\n"
			"vt 0.625000 1.000000\n"
			"vn 0.0000 1.0000 0.0000\n"
			"vn 0.0000 0.0000 1.0000\n"
			"vn -1.0000 0.0000 0.0000\n"
			"vn 0.0000 -1.0000 0.0000\n"
			"vn 1.0000 0.0000 0.0000\n"
			"vn 0.0000 0.0000 -1.0000\n"
			"s off\n"
			"f 5/1/1 3/2/1 1/3/1\n"
			"f 3/2/2 8/4/2 4/5/2\n"
			"f 7/6/3 6/7/3 8/8/3\n"
			"f 2/9/4 8/10/4 6/11/4\n"
			"f 1/3/5 4/5/5 2/9/5\n"
			"f 5/12/6 2/9/6 6/7/6\n"
			"f 5/1/1 7/13/1 3/2/1\n"
			"f 3/2/2 7/14/2 8/4/2\n"
			"f 7/6/3 5/12/3 6/7/3\n"
			"f 2/9/4 4/5/4 8/10/4\n"
			"f 1/3/5 3/2/5 4/5/5\n"
			"f 5/12/6 1/3/6 2/9/6";

	Mesh::Mesh(const RawMesh& data)
			: elementCount(data.indices.size()) {

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &vertexVBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
		glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(Vertex), &data.vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), &data.indices[0],
		             GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));
		// vertex texture coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texUV));
		// vertex normals
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	Mesh::~Mesh() {
		if (VAO) glDeleteVertexArrays(1, &VAO);
		if (vertexVBO) glDeleteBuffers(1, &vertexVBO);
		if (EBO) glDeleteBuffers(1, &EBO);
	}

	void Mesh::linkInstanceTransforms(GLuint vbo) {
		GLint boundVAO;
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
		GLint boundVBO;
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundVBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		for (int i = 10; i < 14; ++i) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) ((i - 10) * sizeof(glm::vec4)));
			glVertexAttribDivisor(i, 1);
		}

		glBindBuffer(GL_ARRAY_BUFFER, boundVBO);
		glBindVertexArray(boundVAO);
	}

	GLuint Mesh::getVao() const {
		return VAO;
	}
}