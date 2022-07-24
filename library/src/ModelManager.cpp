//
// Created by Chris on 02/07/2022.
//

#include "ModelManager.h"

namespace Amber {
    void ModelManager::add(Model& model) {
        if (model.manager == this) return;

        Mesh* mesh = model.mesh;

        if (!instances.contains(mesh)) {
            instances.insert(std::pair(mesh, new std::vector<glm::mat4>()));
            resize.insert(std::pair(mesh, true));
        }

        resize.at(mesh) = true;

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
        if (model.manager != this) return;

        Mesh* mesh = model.mesh;

        resize.at(mesh) = true;

        //copy managed matrix into transform and reset pointer
        //no need to deduce transformation vectors since managed matrix can't be operated on directly, only through vector updates
        ModelTransform* transform = model.transform;
        transform->own = *transform->matrix;
        transform->matrix = &transform->own;

        auto* list = instances.at(mesh);
        list->erase(list->begin() + index.at(&model)); //todo narrowing conversion unsigned -> signed
        index.erase(&model);
    }

    void ModelManager::buffer(Mesh* mesh) {
        if (!instances.contains(mesh)) return;
        glBindBuffer(mesh->getInstanceVbo(), GL_ARRAY_BUFFER);

        auto* matrices = instances.at(mesh);
        if (resize.at(mesh)) {
            resize.at(mesh) = false;
            glBufferData(GL_ARRAY_BUFFER, matrices->size() * sizeof(glm::mat4), &matrices[0], GL_DYNAMIC_DRAW);
        } else {
            glBufferSubData(GL_ARRAY_BUFFER, 0, matrices->size() * sizeof(glm::mat4),
                            &matrices[0]); //todo buffering at no changes :(
        }

    }

    ModelManager::~ModelManager() {
        for (auto pair: instances) {
            delete pair.second;
        }
        instances.clear();
    }

}