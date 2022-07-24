//
// Created by croissant on 09/07/2022.
//

#ifndef ENGINE_ITRANSFORMABLE_H
#define ENGINE_ITRANSFORMABLE_H

#include "glm/glm.hpp"

namespace Amber {
    class ModelTransform;

    template<class C>
    class ITransformable {
        friend C;

        ITransformable() = default;

    public:
        ModelTransform* getTransform() {
            return static_cast<C*>(this)->_getTransform();
        }

        void setTranslation(glm::vec3 v) {
            static_cast<C*>(this)->getTransform()->_setTranslation(v);
        }

        void translate(glm::vec3 v) {
            static_cast<C*>(this)->getTransform()->_translate(v);
        }

        void setRotation(glm::vec3 v) {
            static_cast<C*>(this)->getTransform()->_setRotation(v);
        }

        void rotate(glm::vec3 v) {
            static_cast<C*>(this)->getTransform()->_rotate(v);
        }

        void setScale(glm::vec3 v) {
            static_cast<C*>(this)->getTransform()->_setScale(v);
        }

        void scale(glm::vec3 v) {
            static_cast<C*>(this)->getTransform()->_scale(v);
        }
    };

}

#endif //ENGINE_ITRANSFORMABLE_H
