//
// Created by Chris on 02/07/2022.
//

#ifndef ENGINE_MODELMANAGER_H
#define ENGINE_MODELMANAGER_H

#include <map>
#include <set>
#include <list>
#include "Mesh.h"
#include "ModelTransform.h"
#include "ITransformable.h"
#include "Model.h"

//todo buffer mapping
namespace Amber {

	class ModelManager {
		friend class Model;

		using index = unsigned long long int;

	private:
		std::list<Model> models;
		std::map<Mesh*, std::vector<glm::mat4>*> instances;
		std::map<Mesh*, std::map<index, ModelTransform*>*> references; //maps back to ref of matrix
		std::map<Mesh*, index> pickCount;
		std::map<Mesh*, index> renderCount;
		std::set<Mesh*> resize;

		void updateRef(Mesh* mesh, const index from, const index to);

	public:
		virtual ~ModelManager();

		Model& newModel();

		void reserve(Mesh* mesh, index limit);

		void add(Model& model, index limit);

		void remove(Model& model);

		void swap(Mesh* mesh, index i1, index i2);

		void buffer(Mesh* mesh);

		unsigned long long getRenderCount(Mesh* mesh);

		unsigned long long getPickCount(Mesh* mesh);

	};

}

#endif //ENGINE_MODELMANAGER_H
