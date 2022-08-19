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
		parent->children.remove(this);
		parent = nullptr;
		setMatrix();
	}

	void ModelTransform::propagate() {
		//if parent, if parent.manager, get index of parent.model
		if (parent) {
			glm::mat4 parentMatrix(1);
			if (parent->manager) {
				glBindBuffer(GL_ARRAY_BUFFER, parent->model->getMesh()->getInstanceVbo());
				unsigned int i = parent->manager->getInstanceOffset(*parent->model);
				auto* p = static_cast<glm::mat4*>(glMapBufferRange(GL_ARRAY_BUFFER,
				                                                   i * sizeof(glm::mat4),
				                                                   sizeof(glm::mat4), GL_MAP_READ_BIT));
				parentMatrix = *p;
				glUnmapBuffer(GL_ARRAY_BUFFER);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			} else {
				parentMatrix = parent->own;
			}

			if (manager) {
				glBindBuffer(GL_ARRAY_BUFFER, model->getMesh()->getInstanceVbo());
				unsigned int i = manager->getInstanceOffset(*model);
				auto* p = static_cast<glm::mat4*>(glMapBufferRange(GL_ARRAY_BUFFER, i * sizeof(glm::mat4),
				                                                   sizeof(glm::mat4),
				                                                   GL_MAP_WRITE_BIT | GL_MAP_READ_BIT));
				p[0] = parentMatrix * p[0];
				glUnmapBuffer(GL_ARRAY_BUFFER);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			} else {
				own = parentMatrix * own;
			}
		}
		for (auto child: children) {
			child->setMatrix();
		}
	}

	void ModelTransform::setMatrix() {
		if (manager) {
			glBindBuffer(GL_ARRAY_BUFFER, model->getMesh()->getInstanceVbo());
			unsigned int i = manager->getInstanceOffset(*model);
			auto* p = static_cast<glm::mat4*>(glMapBufferRange(GL_ARRAY_BUFFER, i * sizeof(glm::mat4),
			                                                   sizeof(glm::mat4), GL_MAP_WRITE_BIT));
			*p = t * s * r;
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		} else {
			own = t * s * r;
		}
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

}