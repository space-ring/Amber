//
// Created by croissant on 15/04/2022.
//

#ifndef ENGINE_MODELTRANSFORM_H
#define ENGINE_MODELTRANSFORM_H

#include "glm/glm.hpp"
#include "ITransformable.h"
#include "ITree.h"
#include <forward_list>
#include <functional>

namespace Amber {

	class Transform : public ITree<Transform, Transform, glm::mat4>,
	                  public ITransformable<Transform, glm::vec3> {

		friend ITree<Transform, Transform, glm::mat4>;

		friend ITransformable<Transform, glm::vec3>;

		Transform* parent = nullptr;
		std::forward_list<Transform*> children;

		glm::vec3 translation = glm::vec3(0);
		glm::vec3 rotation = glm::vec3(0);
		glm::vec3 m_scale = glm::vec3(1);

		glm::mat4 own = glm::mat4(1);
		glm::mat4 chained = own;

		void updateMatrix();

		void _attachParent(Transform& parent);

		void _detachParent();

		glm::mat4 _get();

		const glm::mat4* _getP() const;

		void _propagate();

		void _setTranslation(glm::vec3 v);

		void _translate(glm::vec3 v);

		void _setRotation(glm::vec3 v);

		void _rotate(glm::vec3 v);

		void _setScale(glm::vec3 v);

		void _scale(glm::vec3 v);

		void _addScale(glm::vec3 v);

		const glm::vec3& _getTranslation() const;

		const glm::vec3& _getRotation() const;

		const glm::vec3& _getScale() const;

	public:

		std::function<void(Transform&)> _modelChainCallback;

		Transform();

		Transform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);

		~Transform();

		//not safe to do copy or move because they may be managed
		Transform(const Transform&) = delete;

		Transform(Transform&&) = delete;

		Transform& operator=(const Transform&) = delete;

		Transform& operator=(Transform&&) = delete;
	};
}

#endif //ENGINE_MODELTRANSFORM_H
