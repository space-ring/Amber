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

        std::map<Mesh*, std::vector<glm::mat4>*> instances;
        std::map<Model*, unsigned long long> index; //stores index of instance in mapped vec
        std::map<Mesh*, unsigned long> sizes; //stores max instance count
        std::map<Mesh*, bool> resize; //todo just use set
        std::map<Mesh*,
                std::map<unsigned long long, ModelTransform&>*> transforms;

        virtual ~ModelManager();

        void add(Model& model, unsigned long limit); //todo don't like this signature

//        void remove(Model& model);

        void buffer(Mesh* mesh);

        unsigned long getLimit(Mesh* mesh);

    };

}

#endif //ENGINE_MODELMANAGER_H
