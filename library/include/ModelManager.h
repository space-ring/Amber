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

        //todo use std::array and not vector: strictly disallow reallocation
        //this is fine because this is just per layer and at this granularity it shouldn't be hard to estimate
        // how many instances will be alive at once
        std::map<Mesh*, std::vector<glm::mat4>*> instances;
        std::map<Model*, unsigned long long> index; //stores index of instance in mapped vec
        std::map<Mesh*, unsigned long> sizes; //stores max instance count
        std::map<Mesh*, bool> resize; //todo just use set
        std::map<Mesh*,
                std::map<unsigned long long, ModelTransform&>*> transforms;
        std::map<Mesh*, unsigned long long> pickables; //todo

    public:
        virtual ~ModelManager();

        //todo return ref, OWN this model.
        /*
         * Model& cube = models.add(Model());
         */
        void add(Model& model, unsigned long limit); //todo don't like this signature

//        void remove(Model& model);

        void swap(unsigned long long i1, unsigned long long i2);

        void swap(Model& mode, unsigned long long i);

        void buffer(Mesh* mesh);

        unsigned long getLimit(Mesh* mesh);

        unsigned long getCount(Mesh* mesh);

    };

}

#endif //ENGINE_MODELMANAGER_H
