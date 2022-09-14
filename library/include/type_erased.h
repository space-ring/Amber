//
// Created by croissant on 11/09/2022.
//

#ifndef ENGINETEST_TYPE_ERASED_H
#define ENGINETEST_TYPE_ERASED_H

#include <vector>

namespace Amber {
	struct type_erased {
		virtual ~type_erased() {}
	};

	template<class T>
	struct ErasedVector : type_erased {
		std::vector<T> data;
	};
}

#endif //ENGINETEST_TYPE_ERASED_H
