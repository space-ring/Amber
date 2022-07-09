//
// Created by croissant on 09/07/2022.
//

#include "Model.h"
#include <iostream>

unsigned long Model::counter{0};

Model::Model()
        : own(ModelTransform()), transform(&own), id(counter++) {}

Model::Model(Mesh* mesh, ModelTransform& transform)
        : mesh(mesh), own(ModelTransform()), transform(&transform), id(counter++) {}

Mesh* Model::getMesh() const {
    return mesh;
}

void Model::setMesh(Mesh* mesh) {
    Model::mesh = mesh;
}

void Model::setTransform(ModelTransform* transform) {
    Model::transform = transform;
}