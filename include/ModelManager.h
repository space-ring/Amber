//
// Created by Chris on 02/07/2022.
//

#ifndef ENGINE_MODELMANAGER_H
#define ENGINE_MODELMANAGER_H

#include <map>
#include "Mesh.h"
#include "ModelTransform.h"
#include "ITransformable.h"
#include "Model.h"

class ModelManager {

public:

    std::map<Mesh*, std::vector<glm::mat4>*> instances; //todo use lists to optimise removal?
    std::map<Model*, unsigned long long> index;

    void add(Model& model);

    void remove(Model& model);

};


#endif //ENGINE_MODELMANAGER_H
