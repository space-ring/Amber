//
// Created by croissant on 09/07/2022.
//

#include "Model.h"
#include "ModelManager.h"

namespace Amber {
	unsigned long Model::counter{0};

	Model::Model()
			: id(counter++) {}

	Model::Model(Mesh* mesh)
			: mesh(mesh), id(counter++) {}

	Model::~Model() {
		if (manager) manager->remove(*this);
	}

	void Model::setMesh(Mesh* newMesh, unsigned long limit) {
		if (manager) manager->remove(*this);
		this->mesh = newMesh;
		if (manager) manager->add(*this, limit);
	}

	Mesh* Model::getMesh() const {
		return mesh;
	}

	Amber::RenderState Model::getState() const {
		return state;
	}

	void Model::setState(Amber::RenderState state) {
		ModelManager* temp = manager;
		if (manager) manager->remove(*this);
		this->state = state;
		if (temp) temp->add(*this);
	}

	void Model::free() {
		if (manager) manager->remove(*this);
	}
}