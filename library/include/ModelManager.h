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

//todo really need to semi-manage invisibles?
//todo buffer mapping
//todo nodes in trees should be stored together but what if they have different render states?
namespace Amber {

	class ModelManager {
		friend Model;
		friend ModelTransform;

		using index = unsigned long long int;

	private:
		std::list<Model> models;
		std::map<Mesh*, index> limits;
		std::map<Mesh*, std::map<ModelTransform*, index>*> indices; //maps to index in video memory
		std::map<Mesh*, std::map<index, ModelTransform*>*> references; //maps back to ref of matrix
		std::map<Mesh*, index> pickCount;
		std::map<Mesh*, index> renderCount;

		void reserve(Mesh* mesh, const index limit);

		glm::mat4 read(Mesh* mesh, const ModelManager::index index);

		void write(Mesh* mesh, const ModelManager::index index, ModelTransform& transform);

		void copy(Mesh* mesh, const index from, const index to);

		index getInstanceOffset(Model& model);

		void updateRef(Mesh* mesh, const index from, const index to);

	public:

		virtual ~ModelManager();

		Model& newModel();

		void add(Model& model, index limit = 100);

		void remove(Model& model);

		unsigned long long getRenderCount(Mesh* mesh);

		unsigned long long getPickCount(Mesh* mesh);

	};

}

#endif //ENGINE_MODELMANAGER_H
