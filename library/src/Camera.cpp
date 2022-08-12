//
// Created by croissant on 23/05/2022.
//

#include "Camera.h"

namespace Amber {

    Camera::Camera(const glm::vec3& pos, const glm::vec3& rotation,
                   float fov, float ratio, float nearP, float farP, float nearO, float farO)
            : position(pos), rotation(rotation),
              fov(fov), ratio(ratio), near_p(nearP), far_p(farP), near_o(nearO), far_o(farO) {
        updatePerspective();
        updateOrthogonal();
        updateView();
    }

    void Camera::updatePerspective() {
        perspective = glm::perspective(glm::radians(fov), ratio, near_p, far_p);
    }

    void Camera::updateOrthogonal() {
        orthogonal = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near_o, far_o);
    }

    void Camera::updateView() {
        view = glm::translate(glm::mat4(1.0), -position)
               * glm::eulerAngleXYZ(glm::radians(-rotation.x),
                                    glm::radians(-rotation.y),
                                    glm::radians(-rotation.z));
    }

    void Camera::setPerspective(float fov, float ratio, float near, float far) {
        this->fov = fov;
        this->ratio = ratio;
        near_p = near;
        far_p = far;
        perspective = glm::perspective(glm::radians(fov), ratio, near, far);
    }

    void Camera::setOrthogonal(float near, float far) {
        near_o = near;
        far_o = far;
        orthogonal = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near, far);
    }

    void Camera::move(const glm::vec3& d) {
        position += d;
        updateView();
    }

    void Camera::rotate(const glm::vec3& dxyz) {
        rotation += dxyz;
        updateView();
    }

    void Camera::lookAt(glm::vec3 dxyz) {

    }

    const glm::mat4& Camera::getView() const {
        return view;
    }

    const glm::mat4& Camera::getPerspective() const {
        return perspective;
    }

    const glm::mat4& Camera::getOrthogonal() const {
        return orthogonal;
    }
}