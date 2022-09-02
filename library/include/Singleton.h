//
// Created by croissant on 26/08/2022.
//

#ifndef ENGINETEST_SINGLETON_H
#define ENGINETEST_SINGLETON_H

#include <functional>
#include <string>

namespace Amber {

	template<class U>
	class NoDefaultSingleton;

/**
 * A singleton template superclass. The subclass must implement a private default constructor and declare Singleton as a friend.
 * @tparam T - Subclass to be singleton.
 */
	template<class T>
	class Singleton {

		//constructor cannot be called by anyone else (avoid A:Singleton<B>)
		friend T;

		//except NoDefaultSingleton
		friend NoDefaultSingleton<T>;

		Singleton() = default;

		~Singleton() = default;

		//returns whichever partial function constructs instance first
		static T& applyPartial(const std::function<T&()>& function) {
			static T& instance = function();
			return instance;
		}

		template<class... Args>
		static T& instantiate(Args... args) {
			static T instance(std::forward<Args>(args)...);
			return instance;
		}

	public:

		//templated to allow non-default construction of T
		template<class... Args>
		static T& getInstance(Args... args) {
			static auto partial = std::bind(instantiate<Args...>, args...);
			return applyPartial(partial);
		}

		//no available copy or move semantics to call (T cannot be coped or moved)
		Singleton(const Singleton&) = delete;

		Singleton(Singleton&&) = delete;

		Singleton& operator=(const Singleton&) = delete;

		Singleton& operator=(Singleton&&) = delete;
	};
}

#endif //ENGINETEST_SINGLETON_H
