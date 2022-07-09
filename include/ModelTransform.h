//
// Created by croissant on 15/04/2022.
//

#ifndef ENGINE_MODELTRANSFORM_H
#define ENGINE_MODELTRANSFORM_H

#include "glm/glm.hpp"
#include "ITransformable.h"

class Model;

class ModelManager;

class ModelTransform : public ITransformable<ModelTransform> {
    friend class ITransformable<ModelTransform>;

    friend class ITransformable<Model>;

    glm::mat4* matrix;
    glm::mat4 own;

    glm::vec3 translation;
    glm::vec3 rotation; //todo use quaternions
    glm::vec3 scale;

    ModelTransform* _getTransform() {
        return this;
    }

    void updateT();

    void _setTranslation(glm::vec3 v);

    void _translate(glm::vec3 v);

    void _setRotation(glm::vec3 v);

    void _rotate(glm::vec3 v);

    void _setScale(glm::vec3 v);

    void _scale(glm::vec3 v);

public:

    ModelTransform();

    ModelTransform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);

    glm::mat4* getMatrix() const;

    void setMatrix(glm::mat4* matrix);
};


#endif //ENGINE_MODELTRANSFORM_H
