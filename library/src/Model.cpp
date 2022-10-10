//
// Created by croissant on 09/07/2022.
//

#include "Model.h"
#include "Transform.h"

namespace Amber {

	Model::Model(Mesh* mesh, const Transform& t)
			: mesh(mesh), transform(t.getTranslation(), t.getRotation(), t.getScale()) {
	}

	Model::~Model() {
		//todo
		if (_directorRemoveCallback)
			_directorRemoveCallback(*this);
	}

	const Mesh* Model::getMesh() const {
		return mesh;
	}

	void Model::setMesh(Mesh* m) {
		auto f = _directorAddCallback;
		if (_directorRemoveCallback)
			_directorRemoveCallback(*this);
		mesh = m;
		if (f)
			f(*this);
	}

	RenderState Model::getRenderState() const {
		return state;
	}

	void Model::setRenderState(RenderState s) {
		auto f = _directorAddCallback;
		if (_directorRemoveCallback)
			_directorRemoveCallback(*this);
		state = s;
		if (f)
			f(*this);
	}
}