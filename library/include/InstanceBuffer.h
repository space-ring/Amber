//
// Created by Chris on 02/07/2022.
//

#ifndef ENGINE_MODELMANAGER_H
#define ENGINE_MODELMANAGER_H

#include <map>
#include <set>
#include <list>
#include <limits>
#include "Mesh.h"
#include "Transform.h"
#include "ITransformable.h"
#include "Model.h"

//todo really need to semi-manage invisibles?
//todo buffer mapping
//todo nodes in trees should be stored together but what if they have different render states?
namespace Amber {

	struct Tracker {
		using index = long long int;
		index min = LLONG_MAX, max = 0;
		std::list<index> list;

		void track(index i) {
			if (i < min) min = i;
			if (i > max) max = i;
			list.push_back(i);
		}

		void untrack(index i) {
			//don't change min, max too expensive. Doesn't matter for job division anyway
			list.remove(i);
		}
	};

	class InstanceBuffer {

		using index = Tracker::index;

		/**
		 * memory - instance vbo for this director<br>
		 * capacity - size of vbo<br>
		 * size - number of instances in vbo<br>
		 * visible - number of instances with VISIBLE state<br>
		 * solid - number of instances with VISIBLE_SOLID state<br>
		 */
		struct Managed {
			GLuint memory;
			index capacity, size, visible, solid;
		};

		std::list<Model> models;
		std::map<const Mesh*, Managed> managed;
		std::map<const Mesh*, std::map<const Transform*, index>*> indices; //maps to index in video memory
		std::map<const Mesh*, std::map<index, const Transform*>*> references; //maps back to ref of matrix
		std::map<const Mesh*, Tracker> trackers;

		glm::mat4 readGPU(const Mesh* mesh, index i);

		void writeGPU(const Mesh* mesh, index i);

		void copyGPU(const Mesh* mesh, index from, index to);

		/**
		 * Erases old pair and sets new pair in references,
		 * sets new pair in indices,
		 * untracks old index and tracks new index
		 * all without checks.
		 */
		void moveRef(const Mesh* mesh, index from, index to);

	public:

		~InstanceBuffer();

		bool addMesh(const Mesh* mesh, index limit);

		Model& newModel();

		bool addModel(Model& model);

		void removeModel(Model& model);

		void track(const Mesh* m, const Transform& t);

		void untrack(const Mesh* m, const Transform& t);

		index getRenderCount(Mesh* mesh);

		index getPickCount(Mesh* mesh);

		void buffer(const Mesh* mesh);

		void link(Mesh* mesh);

	};

}

#endif //ENGINE_MODELMANAGER_H
