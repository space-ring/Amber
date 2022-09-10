//
// Created by croissant on 10/09/2022.
//

#ifndef ENGINETEST_ICLONABLE_H
#define ENGINETEST_ICLONABLE_H
namespace Amber {
	template<class A, class B>
	struct IClonable : A {
		template<class... Args>
		IClonable(Args... args) : A(args...) {}

		A* clone() const override {
			return new B(static_cast<const B&>(*this));
		}
	};
}

#endif //ENGINETEST_ICLONABLE_H
