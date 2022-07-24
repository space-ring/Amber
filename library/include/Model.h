//
// Created by croissant on 09/07/2022.
//

#ifndef ENGINE_MODEL_H
#define ENGINE_MODEL_H


#include "Mesh.h"
#include "ModelTransform.h"

namespace Amber {
    class ModelManager;

    class Model : public ITransformable<Model> {
        friend class ITransformable<Model>;

        friend class ModelManager;

        static unsigned long counter;
        ModelManager* manager{nullptr}; //todo add to multiple managers?
        Mesh* mesh{nullptr};

        ModelTransform own;
        ModelTransform* transform;

        ModelTransform* _getTransform() { return transform; }

    public:
        const unsigned long id;

        Model();

        Model(Mesh* mesh, ModelTransform& transform);

    };

}


#endif //ENGINE_MODEL_H
