//
// Created by croissant on 15/04/2022.
//

#include "ModelTransform.h"
#include "maths.h"
#include "ModelManager.h"

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
			  own(glm::mat4(1)) { setMatrix(); }

	ModelTransform::ModelTransform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
			: translation(translation), rotation(rotation), m_scale(scale),
			  t(glm::translate(translation)),
			  s(glm::scale(scale)),
			  r(glm::eulerAngleXYX(rotation.x, rotation.y, rotation.z)),
			  own(glm::mat4(1)) { setMatrix(); }


	void ModelTransform::attachParent(ModelTransform& transform, bool inherit) {
		if (parent) detachParent(inherit);
		parent = &transform;
		parent->children.push_back(this);
		//todo set s,r,t to diff so there's no jump at attachment (change of relativity)
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
		parent->children.remove(this);
		parent = nullptr;
	}

	ModelTransform* ModelTransform::root() {
		if (parent) return parent->root();
		return this;
	}

	void ModelTransform::propagate() {
		if (parent) {
			chained = parent->chained * own;
		}
		if (manager) {
//			auto position =manager->indices.at(model->getMesh())->at(this);
//			auto& tracker = manager->trackers.at(model->getMesh());
//			tracker.list.push_back(position);
			manager->trackers.at(model->getMesh()).track(manager->indices.at(model->getMesh())->at(this));

//			auto it = tracker->begin();
//			std::advance(it, position);
//			tracker->insert(it, position);
		}
		for (auto child: children) {
			child->propagate();
		}
	}

	void ModelTransform::setMatrix() {
		own = t * s * r;
		chained = own;
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

}