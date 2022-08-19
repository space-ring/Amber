//
// Created by croissant on 15/04/2022.
//

#ifndef ENGINE_MODELTRANSFORM_H
#define ENGINE_MODELTRANSFORM_H

#include "glm/glm.hpp"
#include "ITransformable.h"
#include <list>

//todo detach from dead parent

namespace Amber {
	class Model;

	class ModelManager;

	class ModelTransform : public ITransformable<ModelTransform> { //this is wrong usage of crtp
		friend class ITransformable<ModelTransform>;

		friend class ITransformable<Model>;

		friend class ModelManager;

		friend class Model;

		ModelTransform* parent = nullptr;
		ModelManager* manager = nullptr;
		Model* model = nullptr;
		std::list<ModelTransform*> children;
	public:
		glm::mat4 own;

		glm::vec3 translation;
		glm::vec3 rotation; //todo use quaternions
		glm::vec3 m_scale;

		glm::mat4 t, r, s;

		ModelTransform* _getTransform() {
			return this;
		}

		void _setTranslation(glm::vec3 v);

		void _translate(glm::vec3 v);

		void _setRotation(glm::vec3 v);

		void _rotate(glm::vec3 v);

		void _setScale(glm::vec3 v);

		void _scale(glm::vec3 v);

		void setMatrix();

		void propagate();

		ModelTransform sumTree();

	public:

		ModelTransform();

		ModelTransform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);

		void attachParent(ModelTransform& transform, bool inherit);

		void detachParent(bool inherit);

	};

}

#endif //ENGINE_MODELTRANSFORM_H
