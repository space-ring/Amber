//
// Created by Chris on 02/07/2022.
//

#ifndef ENGINE_MODELMANAGER_H
#define ENGINE_MODELMANAGER_H

#include <map>
#include "Mesh.h"
#include "ModelTransform.h"
#include "Model.h"

class ModelManager {

public:

    std::map<Mesh*, std::vector<glm::mat4>*> instances;

    void add(Model& model) {
        Mesh* mesh = model.getMesh();
        if (!instances.contains(mesh))
            instances.insert(std::pair(mesh, new std::vector<glm::mat4>()));

        auto* list = instances.at(mesh);
        list->push_back(*model.getTransform()->getMatrix());

        //change Transform's pointer (since vec stores copies)
        model.getTransform()->setMatrix(&list->at(list->size() - 1));
    }

    void remove(const Model& model) {

    }

};


#endif //ENGINE_MODELMANAGER_H
