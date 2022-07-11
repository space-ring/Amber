//
// Created by Chris on 02/07/2022.
//

#include "ModelManager.h"

void ModelManager::add(Model& model) {
    if (index.contains(&model)) return;

    Mesh* mesh = model.mesh;

    if (!instances.contains(mesh))
        instances.insert(std::pair(mesh, new std::vector<glm::mat4>()));

    auto* list = instances.at(mesh);
    list->push_back(*model.transform->matrix);

    //change Transform's pointer (since vec stores copies)
    auto position = list->size() - 1;
    model.transform->matrix = &list->at(position);

    //add model to index
    index.insert(std::pair(&model, position));
    model.manager = this;
}

void ModelManager::remove(Model& model) {
    if (!index.contains(&model)) return;

    Mesh* mesh = model.mesh;

    //copy managed matrix into transform and reset pointer
    //no need to deduce transformation vectors since managed matrix can't be operated on directly, only through vector updates
    ModelTransform* transform = model.transform;
    transform->own = *transform->matrix;
    transform->matrix = &transform->own;

    auto* list = instances.at(mesh);
    list->erase(list->begin() + index.at(&model)); //todo narrowing conversion unsigned -> signed
    index.erase(&model);
}
