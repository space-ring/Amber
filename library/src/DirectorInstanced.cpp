//
// Created by Chris on 02/07/2022.
//

#include "DirectorInstanced.h"
#include "Model.h"
#include "Timer.h"

namespace Amber {

	DirectorInstanced::~DirectorInstanced() {//todo
		models.clear();
		for (auto& pair: indices) {
			delete pair.second;
		}
		for (auto& pair: references) {
			delete pair.second;
		}
	}

	Model& DirectorInstanced::newModel() {
		return models.emplace_back();
	}

	void DirectorInstanced::moveRef(const Mesh* mesh, index from, index to) {
		auto* refs = references.at(mesh);
		auto* transform = refs->at(from);
		refs->erase(from);
		refs->insert_or_assign(to, transform);
		indices.at(mesh)->insert_or_assign(transform, to);
		//if some transforms are tracked before their indices change, tracker has stale data.
		//have to remove old index in case it is now a hole (no longer in refs, seg fault)
		auto& tracker = trackers.at(mesh);
		tracker.track(to);
		tracker.untrack(from);
	}

	glm::mat4 DirectorInstanced::readGPU(const Mesh* mesh, index i) {
		glm::mat4 ret(0);
		if (indices.contains(mesh) && references.at(mesh)->contains(i)) {
			glBindBuffer(GL_ARRAY_BUFFER, managed.at(mesh).memory);
			glGetBufferSubData(GL_ARRAY_BUFFER, i * sizeof(glm::mat4), sizeof(glm::mat4), &ret);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		return ret;
	}

	void DirectorInstanced::writeGPU(const Mesh* mesh, index i) {
		if (!indices.contains(mesh) || !references.at(mesh)->contains(i)) return;
		glBindBuffer(GL_ARRAY_BUFFER, managed.at(mesh).memory);
		glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(glm::mat4), sizeof(glm::mat4), references.at(mesh)->at(i)->getP());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void DirectorInstanced::copyGPU(const Mesh* mesh, index from, index to) {
		if (!indices.contains(mesh)) return;
		auto& m = managed.at(mesh);
		GLsizeiptr size = sizeof(glm::mat4);
		glBindBuffer(GL_COPY_READ_BUFFER, m.memory);
		glBindBuffer(GL_COPY_WRITE_BUFFER, m.memory);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, size * from, size * to, size);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	}

	void DirectorInstanced::track(const Mesh* m, const Transform& t) {
		if (!trackers.contains(m)) return;
		trackers.at(m).track(indices.at(m)->at(&t));
	}

	void DirectorInstanced::untrack(const Mesh* m, const Transform& t) {
		if (!trackers.contains(m)) return;
		trackers.at(m).untrack(indices.at(m)->at(&t));
	}

	bool DirectorInstanced::addMesh(const Mesh* mesh, index limit) {
		if (indices.contains(mesh)) {
			if (indices.at(mesh)->size() < limit)
				//cannot request smaller block
				return false;

			//request bigger block
			Managed& m = managed.at(mesh);
			m.capacity = limit;
			//todo might be quicker: copy into new vbo, reallocate storage and map, copy using mapped buffers
			glm::mat4 copy[limit];
			glBindBuffer(GL_ARRAY_BUFFER, m.memory);
			auto* from = static_cast<glm::mat4*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, m.size * sizeof(glm::mat4),
			                                                      GL_MAP_READ_BIT));
			for (int i = 0; i < m.memory; ++i) {
				copy[i] = from[i];
			}
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBufferData(GL_ARRAY_BUFFER, limit * sizeof(glm::mat4), &copy, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		} else {
			//request new block
			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, limit * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			managed.emplace(mesh, Managed{vbo, limit, 0, 0, 0});
			indices.insert(std::pair(mesh, new std::map<const Transform*, index>));
			references.insert(std::pair(mesh, new std::map<index, const Transform*>));
			trackers.insert(std::pair(mesh, Tracker{limit, 0}));
		}
		return true;
	}

	bool DirectorInstanced::addModel(Model& model) {

		auto* mesh = model.getMesh();
		if (!mesh) return false;

		//if no video memory allocated
		if (!managed.contains(mesh)) return false;
		Managed& m = managed.at(mesh);
		//if memory full
		if (m.size >= m.capacity) return false;
		//if already added
		if (indices.at(mesh)->contains(&model.transform)) return false;

		model._directorAddCallback = [&](Model& m) {
			addModel(m);
		};
		model._directorRemoveCallback = [&](Model& m) {
			removeModel(m);
		};
		model.transform._modelChainCallback = [&](const Transform& t) {
			track(mesh, t);
		};

		index position = m.size;
		auto* list = indices.at(mesh);

		switch (model.getRenderState()) {
			case RenderState::VISIBLE_SOLID:
				//if INVISIBLE in list
				if (m.solid + m.visible < list->size()) {
					copyGPU(mesh, m.solid + m.visible, m.size);
					moveRef(mesh, m.solid + m.visible, m.size);
				}

				//if VISIBLE in list
				if (m.visible > 0) {
					copyGPU(mesh, m.solid, m.solid + m.visible);
					moveRef(mesh, m.solid, m.solid + m.visible);
				}

				position = m.solid;
				++m.solid;
				break;

			case RenderState::VISIBLE:
				//if INVISIBLE in list
				if (m.solid + m.visible < list->size()) {
					copyGPU(mesh, m.solid + m.visible, m.size);
					moveRef(mesh, m.solid + m.visible, m.size);
				}

				position = m.solid + m.visible;
				++m.visible;
				break;

			case RenderState::INVISIBLE:
				break;
		}

		indices.at(mesh)->emplace(&model.transform, position);
		references.at(mesh)->emplace(position, &model.transform);
		++m.size;
		writeGPU(mesh, position);
		return true;
		glCheckError();
	}

	void DirectorInstanced::removeModel(Model& model) {
		const Mesh* mesh = model.getMesh();
		if (!mesh) return;

		//if no video memory allocated
		if (!managed.contains(mesh)) return;
		Managed& m = managed.at(mesh);

		model._directorRemoveCallback = std::function<void(Model&)>();
		model._directorAddCallback = std::function<void(Model&)>();
		model.transform._modelChainCallback = std::function<void(Transform&)>();
		untrack(mesh, model.transform);

		auto* list = indices.at(mesh);
		index position = list->at(&model.transform);

		references.at(mesh)->erase(position);
		list->erase(&model.transform);

		switch (model.getRenderState()) {
			case RenderState::VISIBLE_SOLID:
				//swap with the last SOLID
				if (position < m.solid - 1) {
					copyGPU(mesh, m.solid - 1, position);
					moveRef(mesh, m.solid - 1, position);
				}

				//if VISIBLE in list
				if (m.visible > 0) {
					copyGPU(mesh, m.solid + m.visible - 1, m.solid - 1);
					moveRef(mesh, m.solid + m.visible - 1, m.solid - 1);
				}

				//if INVISIBLE in list
				if (m.solid + m.visible < m.size) {
					copyGPU(mesh, m.size - 1, m.solid + m.visible - 1);
					moveRef(mesh, m.size - 1, m.solid + m.visible - 1);
				}

				--m.solid;
				break;

			case RenderState::VISIBLE:
				//swap with the last VISIBLE
				if (position < m.solid + m.visible - 1) {
					copyGPU(mesh, m.solid + m.visible - 1, position);
					moveRef(mesh, m.solid + m.visible - 1, position);
				}

				//if INVISIBLE in list
				if (m.solid + m.visible < m.size) {
					copyGPU(mesh, m.size - 1, m.solid + m.visible - 1);
					moveRef(mesh, m.size - 1, m.solid + m.visible - 1);
				}

				--m.visible;
				break;

			case RenderState::INVISIBLE:
				//swap with the last INVISIBLE
				if (position < m.size - 1) {
					copyGPU(mesh, m.size - 1, position);
					moveRef(mesh, m.size - 1, position);
				}
				break;
		}

		--m.size;
	}

	//upload only changed models
	//should be called after tree propagation
	void DirectorInstanced::buffer(const Mesh* mesh) {
		auto* refs = references.at(mesh);
		auto& tracker = trackers.at(mesh);
		if (tracker.list.empty()) return;
		//todo writing to unsorted memory loses around 0.0008s on i3-10105f
//		shuffle(tracker.list);
//		tracker.list.sort();
		Managed& m = managed.at(mesh);
		glBindBuffer(GL_ARRAY_BUFFER, m.memory);
		auto* video = static_cast<glm::mat4*>(glMapBufferRange(GL_ARRAY_BUFFER, tracker.min,
		                                                       tracker.max - tracker.min + 1,
		                                                       GL_MAP_WRITE_BIT)); //todo off by 1
		for (auto i: tracker.list) {
//			memcpy(video + i, &refs->at(i)->chained, sizeof(glm::mat4));
			video[i] = *refs->at(i)->getP();
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		tracker.min = m.capacity;
		tracker.max = 0;
		tracker.list = std::list<index>();
	}

	void DirectorInstanced::link(Mesh* mesh) {
		if (!mesh) return;
		if (!managed.contains(mesh)) return;
		mesh->linkInstanceTransforms(managed.at(mesh).memory);
	}

	DirectorInstanced::index DirectorInstanced::getRenderCount(Mesh* mesh) {
		if (!managed.contains(mesh))
			return 0;
		Managed& m = managed.at(mesh);
		return m.solid + m.visible;
	}

	DirectorInstanced::index DirectorInstanced::getPickCount(Mesh* mesh) {
		if (!managed.contains(mesh))
			return 0;
		Managed& m = managed.at(mesh);
		return m.solid;
	}

}