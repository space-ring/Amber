//
// Created by Chris on 02/07/2022.
//

#include "ModelManager.h"
#include "maths.h"

namespace Amber {
    void ModelManager::add(Model& model, unsigned long limit) {//todo don't like this limit here
        if (model.manager == this) return;
        model.manager = this;

        Mesh* mesh = model.mesh;

        std::vector<glm::mat4>* list;
        std::map<unsigned long long, ModelTransform&>* refs;

        if (!instances.contains(mesh)) { //first time mesh
            list = new std::vector<glm::mat4>();
            instances.insert(std::pair(mesh, list));
            sizes.insert(std::pair(mesh, limit));
            resize.insert(std::pair(mesh, true));
            refs = new std::map<unsigned long long, ModelTransform&>();
            transforms.insert(std::pair(mesh, refs));
            list->reserve(limit);
        } else {
            list = instances.at(mesh);
            refs = transforms.at(mesh);
        }
        //prevent resizing to keep references
        //todo (what??) using vectors for now, erase type of instance list and use normal array maybe
        if (instances.at(mesh)->size() >= sizes.at(mesh)) return; //todo throw exception

        list->push_back(*model.transform->matrix);

        //change Transform's pointer (since vec stores copies)
        auto position = list->size() - 1;
        model.transform->matrix = &list->at(position);

        //add model to index
        index.insert(std::pair(&model, position));

        resize.at(mesh) = true;
    }

    /*void ModelManager::remove(Model& model) { //todo update references by clever deleting (swap with last, etc)
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
        model.manager = nullptr;
    }*/

    void ModelManager::buffer(Mesh* mesh) { //todo mapping buffers !!! map to the instance vector
        if (!instances.contains(mesh)) return;
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getInstanceVbo());

        auto* models = instances.at(mesh);
        if (resize.at(mesh)) {
            resize.at(mesh) = false;
            glBufferData(GL_ARRAY_BUFFER, models->size() * sizeof(glm::mat4), &((*models)[0]), GL_DYNAMIC_DRAW);
        } else {
            //todo have to copy capacity to overwrite removed items. but this is expensive
            glBufferSubData(GL_ARRAY_BUFFER, 0, models->size() * sizeof(glm::mat4),
                            &((*models)[0])); //todo buffering at no changes :(
            //todo ^^^ solved by buffer mapping
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ModelManager::~ModelManager() {
        for (auto pair: instances) {
            delete pair.second;
        }
    }

    unsigned long ModelManager::getLimit(Mesh* mesh) {
        if (sizes.contains(mesh)) return sizes.at(mesh);
        return 0;
    }

    unsigned long ModelManager::getCount(Mesh* mesh) {
        unsigned long count = 0;
        if (instances.contains(mesh))
            count = instances.at(mesh)->size();
        return count;
    }
}