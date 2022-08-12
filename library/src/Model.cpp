//
// Created by croissant on 09/07/2022.
//

#include "Model.h"
#include "ModelManager.h"
#include <iostream>

namespace Amber {
    unsigned long Model::counter{0};

    Model::Model()
            : own(ModelTransform()), transform(&own), id(counter++) {}

    Model::Model(Mesh* mesh)
            : mesh(mesh), own(ModelTransform()), transform(&own), id(counter++) {}

    Model::Model(Mesh* mesh, ModelTransform& transform)
            : mesh(mesh), own(ModelTransform()), transform(&transform), id(counter++) {}

    void Model::setMesh(Mesh* newMesh, unsigned long limit) {
        ModelManager* temp = manager;
//        if (manager) manager->remove(*this);
        this->mesh = newMesh;
        if (temp) temp->add(*this, limit);
    }

    Mesh* Model::getMesh() const {
        return mesh;
    }
}