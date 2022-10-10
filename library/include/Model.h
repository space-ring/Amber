//
// Created by croissant on 09/07/2022.
//

#ifndef ENGINE_MODEL_H
#define ENGINE_MODEL_H

#include "Mesh.h"
#include "Transform.h"

namespace Amber {

	enum class RenderState {
		VISIBLE_SOLID,
		VISIBLE,
		INVISIBLE
	};

	class Model {

		Mesh* mesh = nullptr;
		RenderState state = RenderState::VISIBLE;

	public:
		std::function<void(Model&)> _directorRemoveCallback;
		std::function<void(Model&)> _directorAddCallback;

		Transform transform;

		Model() = default;

		Model(Mesh* mesh, const Transform& t);

		~Model();

		//not safe to do copy or move because they may be managed
		Model(const Model&) = delete;

		Model(Model&&) = delete;

		Model& operator=(const Model&) = delete;

		Model& operator=(Model&&) = delete;

		const Mesh* getMesh() const;

		void setMesh(Mesh* m);

		RenderState getRenderState() const;

		void setRenderState(RenderState s);
	};
}


#endif //ENGINE_MODEL_H
