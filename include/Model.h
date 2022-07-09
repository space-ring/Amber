//
// Created by croissant on 09/07/2022.
//

#ifndef ENGINE_MODEL_H
#define ENGINE_MODEL_H


#include "Mesh.h"
#include "ModelTransform.h"

class ModelManager;

class Model : public ITransformable<Model> {

    friend class ITransformable<Model>;

    static unsigned long counter;
    ModelManager* manager{nullptr};
    Mesh* mesh{nullptr};

    ModelTransform* transform;
    ModelTransform own;

    ModelTransform* _getTransform() { return transform; }

public:
    const unsigned long id;

    Model();

    Model(Mesh* mesh, ModelTransform& transform);

    void setTransform(ModelTransform* transform);

    Mesh* getMesh() const;

    void setMesh(Mesh* mesh);

};


#endif //ENGINE_MODEL_H
