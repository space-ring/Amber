//
// Created by croissant on 28/05/2022.
//

#ifndef ENGINE_VARIADIC_H
#define ENGINE_VARIADIC_H

#include <functional>

namespace Amber {
	template<class Derived, class Return, class... Args>
	class Variadic {
		friend Derived;
		using f = std::function<Return(Args...)>;

		Variadic(const f& function) : function(function) {}

	protected:
		const f function;

	public:
		Return operator()(Args... args) {
			return static_cast<Derived*>(this)->call(args...);
		}
	};

}

#endif //ENGINE_VARIADIC_H
