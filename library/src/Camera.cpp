//
// Created by croissant on 23/05/2022.
//

#include "Camera.h"

namespace Amber {

    Camera::Camera(const glm::vec3& pos, const glm::vec3& rotation, float fov, float width, float height,
                   float nearP, float farP, float nearO, float farO)
            : position(pos), rotation(rotation),
              fov(fov), width(width), height(height), near_p(nearP), far_p(farP), near_o(nearO), far_o(farO) {
        updatePerspective();
        updateOrthogonal();
        updateView();
    }

    void Camera::updatePerspective() {
        perspective = glm::perspective(glm::radians(fov), width / height, near_p, far_p);
    }

    void Camera::updateOrthogonal() {
        orthogonal = glm::ortho(-width / 2, width / 2, -height / 2, height / 2, near_o, far_o);
    }

    void Camera::updateView() {
        view = glm::translate(glm::mat4(1.0), -position)
               * glm::eulerAngleXYZ(glm::radians(-rotation.x),
                                    glm::radians(-rotation.y),
                                    glm::radians(-rotation.z));
    }

    void Camera::setPerspective(float fov, float width, float height, float near, float far) {
        this->fov = fov;
        this->width = width;
        this->height = height;
        near_p = near;
        far_p = far;
        perspective = glm::perspective(glm::radians(fov), width / height, near, far);
    }

    void Camera::setOrthogonal(float width, float height, float near, float far) {
        this->width = width;
        this->height = height;
        near_o = near;
        far_o = far;
        orthogonal = glm::ortho(0.0f, width, 0.0f, height, near, far);
    }

    void Camera::move(const glm::vec3& d) {
        position += d;
        updateView();
    }

    void Camera::rotate(const glm::vec3& dxyz) {
        rotation += dxyz;
        updateView();
    }

    void Camera::lookAt(glm::vec3 dxyz) {//todo

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

    void Camera::setAspectRatio(float width, float height) {
        if (width == 0 || height == 0) return; //glm error
        this->width = width;
        this->height = height;
        updatePerspective();
        updateOrthogonal();
    }
}