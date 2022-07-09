//
// Created by croissant on 15/04/2022.
//

#include "ModelTransform.h"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

ModelTransform::ModelTransform()
        : translation(glm::vec3(0)), rotation(glm::vec3(0)), scale(glm::vec3(1)), matrix(&own) { updateT(); }

ModelTransform::ModelTransform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
        : translation(translation), rotation(rotation), scale(scale), matrix(&own) { updateT(); }

void ModelTransform::updateT() {
    glm::mat4 t = glm::translate(translation);
    glm::mat4 s = glm::scale(scale);
    glm::mat4 r = glm::eulerAngleXYX(rotation.x, rotation.y, rotation.z);
    *matrix = t * s * r;
}

void ModelTransform::_setTranslation(glm::vec3 v) {
    translation = v;
    updateT();
}

void ModelTransform::_translate(glm::vec3 v) {
    translation += v;
    updateT();
}

void ModelTransform::_setRotation(glm::vec3 v) {
    rotation = v;
    updateT();
}

void ModelTransform::_rotate(glm::vec3 v) {
    rotation += v;
    updateT();
}

void ModelTransform::_setScale(glm::vec3 v) {
    scale = v;
    updateT();
}

void ModelTransform::_scale(glm::vec3 v) {
    scale *= v;
    updateT();
}

glm::mat4* ModelTransform::getMatrix() const {
    return matrix;
}

void ModelTransform::setMatrix(glm::mat4* matrix) {
    ModelTransform::matrix = matrix;
}
