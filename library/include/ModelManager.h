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

	struct TransformTracker {
		using index = unsigned long long int;
		index min = LONG_LONG_MAX, max = 0;
		std::list<index> list;

		void track(index i) {
			if (i < min) min = i;
			if (i >= max) max = i + 1;
			list.push_back(i);
		}

		void untrack(index i) {
			//don't change min, max too expensive
			list.remove(i);
		}
	};

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
		std::map<Mesh*, TransformTracker> trackers;

		void reserve(Mesh* mesh, index limit);

		glm::mat4 read(Mesh* mesh, ModelManager::index index);

		void write(Mesh* mesh, ModelManager::index index, ModelTransform& transform);

		void copy(Mesh* mesh, index from, index to);

		index getInstanceOffset(Model& model);

		void updateRef(Mesh* mesh, index from, index to);

	public:

		virtual ~ModelManager();

		Model& newModel();

		void add(Model& model, index limit = 100);

		void remove(Model& model);

		unsigned long long getRenderCount(Mesh* mesh);

		unsigned long long getPickCount(Mesh* mesh);

		void buffer(Mesh* mesh);

	};

}

#endif //ENGINE_MODELMANAGER_H
