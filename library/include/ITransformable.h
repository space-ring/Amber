//
// Created by croissant on 09/07/2022.
//

#ifndef ENGINE_ITRANSFORMABLE_H
#define ENGINE_ITRANSFORMABLE_H

#include "glm/glm.hpp"

namespace Amber {

	template<class Impl, class Value>
	class ITransformable {
		friend Impl;

		ITransformable() = default;

		Impl* down() {
			return static_cast<Impl*>(this);
		}

		const Impl* constDown() const {
			return static_cast<const Impl*>(this);
		}

	public:

		void setTranslation(Value v) {
			down()->_setTranslation(v);
		}

		void translate(Value v) {
			down()->_translate(v);
		}

		void setRotation(Value v) {
			down()->_setRotation(v);
		}

		void rotate(Value v) {
			down()->_rotate(v);
		}

		void setScale(Value v) {
			down()->_setScale(v);
		}

		void scale(Value v) {
			down()->_scale(v);
		}

		void addScale(Value v) {
			down()->_addScale(v);
		}

		const Value& getTranslation() const {
			return constDown()->_getTranslation();
		}

		const Value& getRotation() const {
			return constDown()->_getRotation();
		}

		const Value& getScale() const {
			return constDown()->_getScale();
		}
	};

}

#endif //ENGINE_ITRANSFORMABLE_H
