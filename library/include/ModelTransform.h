//
// Created by croissant on 15/04/2022.
//

#ifndef ENGINE_MODELTRANSFORM_H
#define ENGINE_MODELTRANSFORM_H

#include "glm/glm.hpp"
#include "ITransformable.h"
#include <vector>

//todo detach from dead parent

namespace Amber {
    class Model;

    class ModelManager;

    class ModelTransform : public ITransformable<ModelTransform> { //this is wrong usage of crtp
        friend class ITransformable<ModelTransform>;

        friend class ITransformable<Model>;

        friend class ModelManager;

        ModelTransform* parent{nullptr};
        std::vector<ModelTransform*> children;

        glm::mat4 own; //for when not managed
        glm::mat4* matrix; //todo use ref

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

        glm::mat4& getMatrix();

    };

}

#endif //ENGINE_MODELTRANSFORM_H
