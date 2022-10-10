//
// Created by croissant on 10/09/2022.
//

#ifndef ENGINETEST_ICLONABLE_H
#define ENGINETEST_ICLONABLE_H
namespace Amber {

	/**
	 * CRTP for cloning B
	 * @tparam A Superclass
	 * @tparam B Subclass to clone
	 */
	template<class A, class B>
	struct IClonable : A {
		template<class... Args>
		IClonable(Args... args) : A(args...) {}

		/**
		 * Clones the subclass using copyGPU construction
		 * @return A new A pointer of type B
		 */
		A* clone() const override {
			return new B(static_cast<const B&>(*this));
		}
	};
}

#endif //ENGINETEST_ICLONABLE_H
