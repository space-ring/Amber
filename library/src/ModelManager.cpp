//
// Created by Chris on 02/07/2022.
//

#include "ModelManager.h"
#include "maths.h"
#include "Model.h"

namespace Amber {

	void ModelManager::updateRef(Mesh* mesh, const index from, const index to) {
		auto* list = instances.at(mesh);
		auto* refs = references.at(mesh);
		refs->at(from)->matrix = &list->at(to);
		refs->insert_or_assign(to, refs->at(from));
		refs->erase(from);
	}

	Model& ModelManager::newModel() {
		models.emplace_back();
		return models.back();
	}

	void ModelManager::reserve(Mesh* mesh, index limit) {
		if (instances.contains(mesh)) return;

		auto* list = new std::vector<glm::mat4>;
		list->shrink_to_fit();
		list->reserve(limit);
		instances.insert(std::pair(mesh, list));

		//notify to update gl storage
		resize.insert(mesh);
		pickCount.insert(std::pair(mesh, 0));
		renderCount.insert(std::pair(mesh, 0));

		//store references
		references.insert(std::pair(mesh, new std::map<index, ModelTransform*>));
	}

	void ModelManager::add(Model& model, index limit = 0) {
		if (model.manager == this) return;
		model.manager = this;

		Mesh* mesh = model.mesh;
		if (!instances.contains(mesh))
			reserve(mesh, limit);

		std::vector<glm::mat4>* list = instances.at(mesh);

		//prevent resizing to keep references
		if (instances.at(mesh)->size() >= list->capacity()) return; //todo throw exception

		index& endSolid = pickCount.at(mesh);
		index& endVisible = renderCount.at(mesh);
		index position; //index of the new element in list

		//todo update references
		switch (model.state) {

			case RenderState::VISIBLE_SOLID:
				if (endSolid < list->size()) { //if not adding to the back

					if (endVisible < list->size()) {//if invisible, needs to move the first one
						list->push_back(list->at(endVisible));
						updateRef(mesh, endVisible, list->size() - 1);
					}

					if (endSolid < endVisible) { //if visible, needs to move the first one
						// this check is fine after last push because it was already satisfied for the push to happen (inequality remains)
						if (endVisible < list->size()) { //to previously created hole
							list->at(endVisible) = list->at(endSolid);
							updateRef(mesh, endSolid, endVisible);
						} else { //or to the end of the vector
							list->push_back(list->at(endSolid));
							updateRef(mesh, endSolid, list->size() - 1);
						}
					}

					list->at(endSolid) = *model.transform->matrix;

				} else list->push_back(*model.transform->matrix);

				position = endSolid;
				++endSolid;
				++endVisible;
				break;

			case RenderState::VISIBLE:
				//if invisible, needs to move (if not adding to the back also)
				//then insert
				if (endVisible < list->size()) {
					list->push_back(list->at(endVisible));
					updateRef(mesh, endVisible, list->size() - 1);
					list->at(endVisible) = *model.transform->matrix;

				} else list->push_back(*model.transform->matrix);

				position = endVisible;
				++endVisible;
				break;

			case RenderState::INVISIBLE:
				list->push_back(*model.transform->matrix);
				position = list->size() - 1;
				break;
		}

		//change Transform's pointer (since vec stores copies)
		model.transform->matrix = &list->at(position);

		//set reference to transform
		references.at(mesh)->insert_or_assign(position, model.transform);
	}

	void ModelManager::remove(Model& model) {
		if (model.manager != this) return;
		model.manager = nullptr;

		Mesh* mesh = model.mesh;

		auto* list = instances.at(mesh);
		index& endSolid = pickCount.at(mesh);
		index& endVisible = renderCount.at(mesh);
		index position = model.transform->matrix - &list->front();

		//copy managed matrix into transform and reset pointer
		//no need to deduce transformation vectors since managed matrix can't be operated on directly, only through vector updates
		//(so the transform already has the correct transformation vectors)
		//do this before overwriting in the switch below
		ModelTransform* transform = model.transform;
		transform->own = *transform->matrix;
		transform->matrix = &transform->own;

		switch (model.state) {

			case RenderState::VISIBLE_SOLID:
				if (position < list->size() - 1) { //if not removing from the back
					// position < endSolid

					if (position < endSolid - 1) { //if not removing the last solid
						//swap with the last solid
						list->at(position) = list->at(endSolid - 1);
						updateRef(mesh, endSolid - 1, position);
					}
					//first hole is always endSolid-1

					index hole = endSolid - 1;
					if (endSolid < endVisible) { //if visible, needs to move the last one
						list->at(endSolid - 1) = list->at(endVisible - 1);
						updateRef(mesh, endVisible - 1, endSolid - 1);
						hole = endVisible - 1;
					}

					if (endVisible < list->size()) { //if invisible, needs to move the last one
						list->at(hole) = list->back();
						updateRef(mesh, list->size() - 1, hole);
					}
				}

				--endSolid;
				--endVisible;
				break;

			case RenderState::VISIBLE:
				if (position < list->size() - 1) { //if not removing from the back
					//position < endVisible

					if (position < endVisible - 1) { //if not removing the last visible
						//swap with the last visible
						list->at(position) = list->at(endVisible - 1);
						updateRef(mesh, endVisible - 1, position);
					}

					//first hole is always endVisible-1
					if (endVisible < list->size()) { //if invisible, needs to move the last one
						list->at(endVisible - 1) = list->back();
						updateRef(mesh, list->size() - 1, endVisible - 1);
					}
				}

				--endVisible;
				break;

			case RenderState::INVISIBLE:

				if (position < list->size() - 1) { //if not removing from the back
					//swap with the last element
					list->at(position) = list->back();
					updateRef(mesh, list->size() - 1, position);
				}
				break;
		}

		list->pop_back(); //there's always a hole at the end
		references.at(mesh)->erase(list->size() - 1);
	}

	void ModelManager::buffer(Mesh* mesh) { //todo mapping buffers !!! map to the instance vector
		if (!instances.contains(mesh)) return;

		glBindBuffer(GL_ARRAY_BUFFER, mesh->getInstanceVbo());

		auto* models = instances.at(mesh);
		if (resize.contains(mesh)) {
			resize.erase(mesh);
			glBufferData(GL_ARRAY_BUFFER, models->capacity() * sizeof(glm::mat4), &((*models)[0]), GL_DYNAMIC_DRAW);
		} else {
			//todo implement buffer mapping to avoid buffering unchanged data
			glBufferSubData(GL_ARRAY_BUFFER, 0, models->size() * sizeof(glm::mat4), &((*models)[0]));
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void ModelManager::swap(Mesh* mesh, index i1, index i2) {
		if (!instances.contains(mesh)) return;
		auto* list = instances.at(mesh);

		auto size = list->size();
		if (i2 < size) {
			if (i1 < size) {
				glm::mat4 temp = list->at(i2);
				list->at(i2) = list->at(i1);
				list->at(i1) = temp;
			} else return; //cannot swap with null element
		}
	}

	ModelManager::~ModelManager() {
		for (auto pair: instances) {
			delete pair.second;
		}
	}

	unsigned long long ModelManager::getRenderCount(Mesh* mesh) {
		unsigned long long count = 0;
		if (instances.contains(mesh))
			count = renderCount.at(mesh);
		return count;
	}

	unsigned long long ModelManager::getPickCount(Mesh* mesh) {
		unsigned long long count = 0;
		if (instances.contains(mesh))
			count = pickCount.at(mesh);
		return count;
	}

}