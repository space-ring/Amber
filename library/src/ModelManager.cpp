//
// Created by Chris on 02/07/2022.
//

#include "ModelManager.h"
#include "Model.h"
#include "Timer.h"

namespace Amber {

	ModelManager::~ModelManager() {
		models.clear();
		for (auto& pair: indices) {
			delete pair.second;
		}
		for (auto& pair: references) {
			delete pair.second;
		}
	}

	ModelManager::index ModelManager::getInstanceOffset(Model& model) { //todo check if model managed
		return indices.at(model.mesh)->at(&model.transform);
	}

	void ModelManager::updateRef(Mesh* mesh, index from, index to) {
		auto* list = indices.at(mesh);
		auto* refs = references.at(mesh);
		auto& tracker = trackers.at(mesh);
		auto transform = refs->at(from);
		refs->erase(from);
		refs->insert_or_assign(to, transform);
		list->insert_or_assign(transform, to);
		//if some transforms are tracked before their indices change, tracker has stale data.
		//have to remove old index in case it is now a hole (no longer in refs, seg fault)
		tracker.track(to);
		tracker.untrack(from);
	}

	Model& ModelManager::newModel() {
		models.emplace_back();
		return models.back();
	}

	void ModelManager::reserve(Mesh* mesh, index limit) {
		if (indices.contains(mesh)) return;

		glBindBuffer(GL_ARRAY_BUFFER, mesh->getInstanceVbo());
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * limit, nullptr, GL_DYNAMIC_DRAW);

		indices.insert(std::pair(mesh, new std::map<ModelTransform*, index>));
		references.insert(std::pair(mesh, new std::map<index, ModelTransform*>));
		trackers.insert(std::pair(mesh, TransformTracker{limit, 0}));

		pickCount.insert(std::pair(mesh, 0));
		renderCount.insert(std::pair(mesh, 0));

		limits.insert(std::pair(mesh, limit));
	}

	glm::mat4 ModelManager::read(Mesh* mesh, ModelManager::index index) {
		glm::mat4 ret(0);
		if (indices.contains(mesh)) {
			glBindBuffer(GL_ARRAY_BUFFER, mesh->getInstanceVbo());
			glGetBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &ret);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		return ret;
	}

	void ModelManager::write(Mesh* mesh, ModelManager::index index, ModelTransform& transform) {
		if (!indices.contains(mesh)) return;
		glBindBuffer(GL_ARRAY_BUFFER, mesh->getInstanceVbo());
		glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &transform.own);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		indices.at(mesh)->insert_or_assign(&transform, index);
		references.at(mesh)->insert_or_assign(index, &transform);
	}

	void ModelManager::copy(Mesh* mesh, ModelManager::index from, ModelManager::index to) {
		if (!indices.contains(mesh)) return;
		glBindBuffer(GL_COPY_READ_BUFFER, mesh->getInstanceVbo());
		glBindBuffer(GL_COPY_WRITE_BUFFER, mesh->getInstanceVbo());
		GLsizeiptr size = sizeof(glm::mat4);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, size * from, size * to, size);
		glBindBuffer(GL_COPY_READ_BUFFER, 0);
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
		updateRef(mesh, from, to);
	}

	void ModelManager::add(Model& model, index limit) {
		if (model.manager == this) return;

		Mesh* mesh = model.mesh;
		if (!indices.contains(mesh))
			reserve(mesh, limit);

		auto* list = indices.at(mesh);

		//prevent growth
		if (list->size() >= limits.at(mesh)) return; //todo throw exception

		index& endSolid = pickCount.at(mesh);
		index& endVisible = renderCount.at(mesh);
		index position = list->size(); //index of the new element in list

		switch (model.state) {

			case RenderState::VISIBLE_SOLID:
				//if VISIBLE or INVISIBLE exist (if not adding to the back)
				if (endSolid < list->size()) {
					//if invisible, needs to move the first one to the back
					if (endVisible < list->size())
						copy(mesh, endVisible, list->size());

					//if visible, needs to move the first one
					if (endSolid < endVisible)
						copy(mesh, endSolid, endVisible);
				}

				position = endSolid;
				++endSolid;
				++endVisible;
				break;

			case RenderState::VISIBLE:
				//if invisible, needs to move (if not adding to the back also)
				if (endVisible < list->size())
					copy(mesh, endVisible, list->size());

				position = endVisible;
				++endVisible;
				break;

			case RenderState::INVISIBLE:
				break;
		}

		write(mesh, position, model.transform);
		model.setManager(this);
	}

	void ModelManager::remove(Model& model) {
		if (model.manager != this) return;

		Mesh* mesh = model.mesh;

		auto* list = indices.at(mesh);
		index& endSolid = pickCount.at(mesh);
		index& endVisible = renderCount.at(mesh);
		index position = list->at(&model.transform);

		//copy managed matrix into transform and reset pointer
		//no need to deduce transformation vectors since managed matrix can't be operated on directly, only through vector updates
		//(so the transform already has the correct transformation vectors)
		//do this before overwriting in the switch below
		ModelTransform& transform = model.transform;
		auto t = read(mesh, list->at(&model.transform));
		transform.own = t;

		references.at(mesh)->erase(position);
		list->erase(&model.transform);

		switch (model.state) {

			case RenderState::VISIBLE_SOLID: //0 <= position < endSolid <= list.size()
				//if not removing from the back
				if (position < list->size() - 1) {

					//if not removing the last solid
					if (position < endSolid - 1) {
						//swap with the last solid
						copy(mesh, endSolid - 1, position);
					}

					//first hole is always endSolid-1
					index hole = endSolid - 1;
					//if visible, needs to move the last one
					if (endSolid < list->size() && endSolid < endVisible) {
						copy(mesh, endVisible - 1, endSolid - 1);
						hole = endVisible - 1;
					}

					//if invisible, needs to move the last one //todo why?
					if (endVisible < list->size()) {
						copy(mesh, list->size() - 1, hole);
					}
				}

				--endSolid;
				--endVisible;
				break;

			case RenderState::VISIBLE:
				//if not removing from the back
				if (position < list->size() - 1) {

					//if not removing the last visible
					if (position < endVisible - 1) {
						//swap with the last visible
						copy(mesh, endVisible - 1, position);
					}

					//first hole is always endVisible-1
					//if invisible, needs to move the last one //todo why?
					if (endVisible < list->size()) {
						copy(mesh, list->size() - 1, endVisible - 1);
					}
				}

				--endVisible;
				break;

			case RenderState::INVISIBLE:
				break;
		}

		model.setManager(nullptr);
	}

	//upload only changed models
	//should be called after tree propagation
	void ModelManager::buffer(Mesh* mesh) {
		auto* refs = references.at(mesh);
		auto& tracker = trackers.at(mesh);
		if (tracker.list.empty()) return;
		//writing to unsorted memory loses around 0.0008s on i3-10105f
//		shuffle(tracker.list);
//		tracker.list.sort();
		glBindBuffer(GL_ARRAY_BUFFER, mesh->getInstanceVbo());
		auto* video = static_cast<glm::mat4*>(glMapBufferRange(GL_ARRAY_BUFFER, tracker.min,
		                                                       tracker.max - tracker.min, GL_MAP_WRITE_BIT));
		for (auto i: tracker.list) {
//			memcpy(video + i, &refs->at(i)->chained, sizeof(glm::mat4));
			video[i] = refs->at(i)->chained;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		tracker.min = limits.at(mesh);
		tracker.max = 0;
		tracker.list = std::list<index>();
	}

	unsigned long long ModelManager::getRenderCount(Mesh* mesh) {
		unsigned long long count = 0;
		if (indices.contains(mesh))
			count = renderCount.at(mesh);
		return count;
	}

	unsigned long long ModelManager::getPickCount(Mesh* mesh) {
		unsigned long long count = 0;
		if (indices.contains(mesh))
			count = pickCount.at(mesh);
		return count;
	}

}