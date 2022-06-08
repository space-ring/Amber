//
// Created by croissant on 23/05/2022.
//

#include "Camera.h"

Camera::Camera(const glm::vec3& pos, const glm::vec3& up, const glm::vec3& right)
        : pos(pos), up(up), right(right), view(glm::mat4()), projection(glm::mat4()) {
}

void Camera::setPerspective(float fov, float ratio, float near, float far) {
    projection = glm::perspective(glm::radians(fov), ratio, near, far);
}

void Camera::setOrthogonal(float near, float far) {
    projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near, far);
}

const glm::mat4& Camera::getProjection() const {
    return projection;
}

const glm::vec3& Camera::getPos() const {
    return pos;
}

const glm::vec3& Camera::getUp() const {
    return up;
}

const glm::vec3& Camera::getRight() const {
    return right;
}

void Camera::move(glm::vec3& d) {
    pos+=d;
    view = glm::translate(view, pos);
}

void Camera::look(glm::vec3 d) {

}

const glm::mat4& Camera::getView() const {
    return view;
}
