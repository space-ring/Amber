//
// Created by croissant on 23/05/2022.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include "maths.h"

namespace Amber {
    class Camera {

    public:
        glm::vec3 position, rotation;
        glm::mat4 view, perspective, orthogonal;
        float fov, ratio, near_p, far_p, near_o, far_o;

        void updatePerspective();

        void updateOrthogonal();

        void updateView();

    public:

        /**
         * @param pos
         * @param rotation pitch - yaw - roll
         * @param fov
         * @param ratio
         * @param nearP
         * @param farP
         * @param nearO
         * @param farO
         */
        Camera(const glm::vec3& pos, const glm::vec3& rotation, float fov, float ratio, float nearP,
               float farP, float nearO, float farO);

        void setPerspective(float fov, float ratio, float near, float far);

        void setOrthogonal(float near, float far);

        void move(const glm::vec3& dxyz);

        void rotate(const glm::vec3& dxyz);

        void lookAt(glm::vec3 dxyz);

        const glm::mat4& getView() const;

        const glm::mat4& getPerspective() const;

        const glm::mat4& getOrthogonal() const;
    };

}

#endif //ENGINE_CAMERA_H
