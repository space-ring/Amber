//
// Created by croissant on 09/07/2022.
//

#ifndef ENGINE_MODEL_H
#define ENGINE_MODEL_H


#include "Mesh.h"
#include "ModelTransform.h"

namespace Amber {
	class ModelManager;

	enum RenderState {
		VISIBLE_SOLID,
		VISIBLE,
		INVISIBLE
	};

	class Model : public ITransformable<Model> {
		friend class ITransformable<Model>;

        friend class ModelManager;

		static unsigned long counter;
		ModelManager* manager{nullptr}; //todo add to multiple managers? impossible, only 1 ref to mat is stored.
		Mesh* mesh{nullptr};

		ModelTransform own; //for when not managed
		ModelTransform* transform; //todo use refs //todo what if transform is destroyed?
		//todo use shared pointer for transform

		ModelTransform* _getTransform() { return transform; }

		RenderState state = RenderState::VISIBLE_SOLID;

	public:
		const unsigned long id;

		Model();

		Model(Mesh* mesh);

		Model(Mesh* mesh, ModelTransform& transform);

		virtual ~Model();

		//todo copy and move
		Model(const Model&) = delete;

		Model(Model&&) = delete;

		Model& operator=(const Model&) = delete;

		Model& operator=(Model&&) = delete;

		void setMesh(Mesh* newMesh, unsigned long limit);

		Mesh* getMesh() const;

		RenderState getState() const;

		void setState(RenderState state);

		void free();

	};

}


#endif //ENGINE_MODEL_H
