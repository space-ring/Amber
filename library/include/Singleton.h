//
// Created by croissant on 26/08/2022.
//

#ifndef ENGINETEST_SINGLETON_H
#define ENGINETEST_SINGLETON_H

#include <functional>
#include <string>

#define DECNODEFAULT(T) template<> template<> T& Amber::Singleton<T>::getInstance<>();
#define DEFNODEFAULT(T, ...) template<> template<> T& Amber::Singleton<T>::getInstance<>(){ \
    if (!initialised) throw std::runtime_error(std::string("Cannot recall instance of non-initialised ") + #T + " with no default constructor");\
    return Amber::Singleton<T>::getInstance(__VA_ARGS__);}

namespace Amber {
/**
 * A singleton template superclass. Subclass must implement a private default constructor and declare this superclass as a friend.
 * @tparam T - Subclass to be singleton.
 */
	template<class T>
	class Singleton {

		static bool initialised;

		//constructor cannot be called by anyone else (avoid A:Singleton<B>)
		friend T;

		Singleton() { initialised = true; };

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

	template<class T>
	bool Singleton<T>::initialised = false;
}

#endif //ENGINETEST_SINGLETON_H
