//
// Created by Chris on 02/07/2022.
//

#include "ModelManager.h"

namespace Amber {
    void ModelManager::add(Model& model, unsigned long size) {
        if (model.manager == this) return;

        Mesh* mesh = model.mesh;

        //prevent resizing to keep references
        //todo using vectors for now, erase type of instance list and use normal array maybe
        if (instances.at(mesh)->size() == sizes.at(mesh)) return; //todo throw exception

        std::vector<glm::mat4>* list;

        if (!instances.contains(mesh)) { //first time mesh
            list = new std::vector<glm::mat4>();
            instances.insert(std::pair(mesh, list));
            sizes.insert(std::pair(mesh, size));
            resize.insert(std::pair(mesh, true));
            list->reserve(size);
        } else list = instances.at(mesh);

        list->push_back(*model.transform->matrix);

        //change Transform's pointer (since vec stores copies)
        auto position = list->size() - 1;
        model.transform->matrix = &list->at(position);

        //add model to index
        index.insert(std::pair(&model, position));
        model.manager = this;

        resize.at(mesh) = true;
    }

    void ModelManager::remove(Model& model) {
        if (model.manager != this) return;

        Mesh* mesh = model.mesh;

        //copy managed matrix into transform and reset pointer
        //no need to deduce transformation vectors since managed matrix can't be operated on directly, only through vector updates
        ModelTransform* transform = model.transform;
        transform->own = *transform->matrix;
        transform->matrix = &transform->own;

        auto* list = instances.at(mesh);
        list->erase(list->begin() + index.at(&model)); //todo narrowing conversion unsigned -> signed
        index.erase(&model);

        resize.at(mesh) = true;
    }

    void ModelManager::buffer(Mesh* mesh) { //todo mapping buffers !!! map to the mesh vector
        if (!instances.contains(mesh)) return;
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getInstanceVbo());

        auto* models = instances.at(mesh);

        if (resize.at(mesh)) {
            resize.at(mesh) = false;
            glBufferData(GL_ARRAY_BUFFER, models->size() * sizeof(glm::mat4), &models[0], GL_DYNAMIC_DRAW);
        } else {
            //todo have to copy capacity to overwrite removed items. but this is expensive
            glBufferSubData(GL_ARRAY_BUFFER, 0, models->size() * sizeof(glm::mat4),
                            &models[0]); //todo buffering at no changes :(
            //todo ^^^ solved by buffer mapping
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ModelManager::~ModelManager() {
        for (auto pair: instances) {
            delete pair.second;
        }
    }

}