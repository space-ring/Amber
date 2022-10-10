//
// Created by croissant on 21/09/2022.
//

#ifndef ENGINETEST_ITREE_H
#define ENGINETEST_ITREE_H

template<class Impl, class Node, class Value>
class ITree {
	friend Impl;

	ITree() = default;

	Impl* down() {
		return static_cast<Impl*>(this);
	}

	const Impl* constDown() const {
		return static_cast<const Impl*>(this);
	}

public:

	Node& root(){
		return down()->_root();
	}

	void attachParent(Node& p) {
		down()->_attachParent(p);
	}

	void detachParent(){
		down()->_detachParent();
	}

	Value get(){
		return down()->_get();
	}

	const Value* getP() const {
		return constDown()->_getP();
	}

	void propagate(){
		down()->_propagate();
	}
};

#endif //ENGINETEST_ITREE_H
