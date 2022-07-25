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
//todo buffer mapping
namespace Amber {
    class ModelManager {

    public:

        std::map<Mesh*, std::vector<glm::mat4>*> instances; //todo use lists to optimise removal? no, must be contiguous
        std::map<Mesh*, bool> resize;
        std::map<Model*, unsigned long long> index;
        std::map<Mesh*, unsigned long> sizes;

        virtual ~ModelManager();

        void add(Model& model, unsigned long size);

        void remove(Model& model);

        void buffer(Mesh* mesh);

    };

}

#endif //ENGINE_MODELMANAGER_H
