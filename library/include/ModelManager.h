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
        std::map<Model*, unsigned long long> index;
        std::map<Mesh*, unsigned long> sizes;
        std::map<Mesh*, bool> resize;

        virtual ~ModelManager();

        void add(Model& model, unsigned long limit);

        void remove(Model& model);

        void buffer(Mesh* mesh);

        unsigned long getLimit(Mesh* mesh);

    };

    Mesh* Model::getMesh() const {
        return mesh;
    }

}

#endif //ENGINE_MODELMANAGER_H
