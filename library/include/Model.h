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
        ModelManager* manager{nullptr}; //todo add to multiple managers? impossible, only 1 ref to mat is stored.
        Mesh* mesh{nullptr};

        ModelTransform own; //for when not managed
        ModelTransform* transform; //todo use refs

        ModelTransform* _getTransform() { return transform; }

    public:
        const unsigned long id;

        //todo sort out links to transforms when models are added (deep copy)

        Model();

        Model(Mesh* mesh);

        Model(Mesh* mesh, ModelTransform& transform);

        void setMesh(Mesh* newMesh, unsigned long limit);

        Mesh* getMesh() const;

    };

}


#endif //ENGINE_MODEL_H
