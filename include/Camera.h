//
// Created by croissant on 23/05/2022.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include "maths.h"

class Camera { //todo actually encapsulate privates.
private:
    glm::vec3 pos, up, right;
    glm::mat4 view, projection;
    float fov, ratio, near, far;
public:
    Camera(const glm::vec3& pos, const glm::vec3& up, const glm::vec3& right);

    void setPerspective(float fov, float ratio, float near, float far);

    void setOrthogonal(float near, float far);

    void move(glm::vec3& dxyz);

    void look(glm::vec3 dxyz);

    const glm::vec3& getPos() const;

    const glm::vec3& getUp() const;

    const glm::vec3& getRight() const;

    const glm::mat4& getProjection() const;

    const glm::mat4& getView() const;
};


#endif //ENGINE_CAMERA_H
