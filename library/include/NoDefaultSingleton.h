//
// Created by croissant on 02/09/2022.
//

#ifndef ENGINETEST_NODEFAULTSINGLETON_H
#define ENGINETEST_NODEFAULTSINGLETON_H

#include <stdexcept>
#include "Singleton.h"

namespace Amber {
	/**
	 * A singleton template superclass which allows calling getInstance() after instantiation of the subclass with no default constructor.
	 * The subclass must declare Singleton as a friend.
	 * @tparam T Subclass to be singleton without a default constructor.
	 */
	template<class T>
	class NoDefaultSingleton : public Amber::Singleton<T> {

		//constructor can only be called by subclass T
		friend T;
		static bool done;

		NoDefaultSingleton() { done = true; };

	public:
		using Singleton<T>::getInstance;

		static T& getInstance() {
			if (!done)
				throw std::runtime_error(
						std::string("Cannot recall instance of non-initialised ") +
						typeid(T).name() +
						" with no default constructor");
			return Amber::Singleton<T>::applyPartial({}); //call using non-function since already instantiated
		}
	};

	template<class T>
	bool NoDefaultSingleton<T>::done = false;
}

#endif //ENGINETEST_NODEFAULTSINGLETON_H
