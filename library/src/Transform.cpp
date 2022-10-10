//
// Created by croissant on 15/04/2022.
//

#include "Transform.h"
#include "maths.h"

namespace Amber {

	Transform::Transform() {
		updateMatrix();
	}

	Transform::Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
			: translation(translation), rotation(rotation), m_scale(scale) {
		updateMatrix();
	}

	Transform::~Transform() {
		//todo
	}

	void Transform::_propagate() {
		if (parent) {
			chained = parent->chained * own;
			_modelChainCallback(*this);
		}

		for (auto child: children)
			child->_propagate();
	}

	void Transform::updateMatrix() {
		own = glm::translate(translation)
		      * glm::scale(m_scale)
		      * glm::eulerAngleXYZ(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));
		chained = own;
		if (_modelChainCallback)
			_modelChainCallback(*this);
	}

	glm::mat4 Transform::_get() {
		return chained;
	}

	const glm::mat4* Transform::_getP() const {
		return &chained;
	}

	void Transform::_attachParent(Transform& transform) {
		if (parent) detachParent();
		parent = &transform;
		parent->children.push_front(this);
	}

	void Transform::_detachParent() {
		if (!parent) return;
		parent->children.remove(this);
		parent = nullptr;
	}

	void Transform::_setTranslation(glm::vec3 v) {
		translation = v;
		updateMatrix();
	}

	void Transform::_translate(glm::vec3 v) {
		translation += v;
		updateMatrix();
	}

	void Transform::_setRotation(glm::vec3 v) {
		rotation = v;
		updateMatrix();
	}

	void Transform::_rotate(glm::vec3 v) {
		rotation += v;
		updateMatrix();
	}

	void Transform::_setScale(glm::vec3 v) {
		m_scale = v;
		updateMatrix();
	}

	void Transform::_scale(glm::vec3 v) {
		m_scale *= v;
		updateMatrix();
	}

	void Transform::_addScale(glm::vec3 v) {
		m_scale += v;
		updateMatrix();
	}

	const glm::vec3& Transform::_getTranslation() const {
		return translation;
	}

	const glm::vec3& Transform::_getRotation() const {
		return rotation;
	}

	const glm::vec3& Transform::_getScale() const {
		return m_scale;
	}

}