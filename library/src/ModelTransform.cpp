//
// Created by croissant on 15/04/2022.
//

#include "ModelTransform.h"
#include "maths.h"

namespace Amber {
	ModelTransform ModelTransform::sumTree() {
		if (!parent) return *this;
		ModelTransform sum = parent->sumTree();
		sum.rotation += rotation;
		sum.translation += translation;
		sum.m_scale *= m_scale;
		return sum;
	}

	ModelTransform::ModelTransform()
			: translation(glm::vec3(0)), rotation(glm::vec3(0)), m_scale(glm::vec3(1)),
			  t(glm::translate(translation)),
			  s(glm::scale(m_scale)),
			  r(glm::eulerAngleXYX(rotation.x, rotation.y, rotation.z)),
			  own(glm::mat4(1)), matrix(&own) { setMatrix(); }

	ModelTransform::ModelTransform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
			: translation(translation), rotation(rotation), m_scale(scale),
			  t(glm::translate(translation)),
			  s(glm::scale(scale)),
			  r(glm::eulerAngleXYX(rotation.x, rotation.y, rotation.z)),
			  own(glm::mat4(1)), matrix(&own) { setMatrix(); }


	void ModelTransform::attachParent(ModelTransform& transform, bool inherit) {
		if (parent) detachParent(inherit);
		parent = &transform;
		parent->children.push_back(
				this); //todo set s,r,t to diff so there's no jump at attachment (change of relativity)
		propagate();
	}

	void ModelTransform::detachParent(bool inherit) {
		if (!parent) return;
		if (inherit) { //if the child should copy the parent's transform at detachment
			ModelTransform sum = sumTree();
			translation = sum.translation;
			t = glm::translate(translation);
			rotation = sum.rotation;
			r = glm::eulerAngleXYX(rotation.x, rotation.y, rotation.z);
			m_scale = sum.m_scale;
			s = glm::scale(m_scale);
		}
		auto it = std::find(parent->children.begin(), parent->children.end(), this);
		std::swap(*it, parent->children.back());
		parent->children.pop_back();
		parent = nullptr;
		setMatrix();
	}

	void ModelTransform::propagate() {
		if (parent) *matrix = *parent->matrix * *matrix; //todo move this line to set_matrix?
		for (auto child: children) {
			child->setMatrix();
		}
	}

	void ModelTransform::setMatrix() {
		*matrix = t * s * r;
		propagate();
	}

	void ModelTransform::_setTranslation(glm::vec3 v) {
		translation = v;
		t = glm::translate(translation);
		setMatrix();
	}

	void ModelTransform::_translate(glm::vec3 v) {
		translation += v;
		t = glm::translate(translation);
		setMatrix();
	}

	void ModelTransform::_setRotation(glm::vec3 v) {
		rotation = v;
		r = glm::eulerAngleXYZ(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));
		setMatrix();
	}

	void ModelTransform::_rotate(glm::vec3 v) {
		rotation += v;
		r = glm::eulerAngleXYZ(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));
		setMatrix();
	}

	void ModelTransform::_setScale(glm::vec3 v) {
		m_scale = v;
		s = glm::scale(m_scale);
		setMatrix();
	}

	void ModelTransform::_scale(glm::vec3 v) {
		m_scale *= v;
		s = glm::scale(v);
		setMatrix();
	}

	glm::mat4& ModelTransform::getMatrix() {
		return *matrix;
	}

}