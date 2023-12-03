#ifndef __STACK_H__
#define __STACK_H__

#include <vector>

template <typename DataType>
class Stack : private std::vector<DataType> {
public:
	void Push(const DataType& node) {
		std::vector<DataType>::push_back(node);
	}
	
	void Pop() {
		std::vector<DataType>::pop_back();
	}

	const DataType& Peek(void) const {
		return std::vector<DataType>::back();
	}
	
	bool Empty() const {
		return std::vector<DataType>::empty();
	}
	
	size_t Size() const {
		return std::vector<DataType>::size();
	}
};

#endif	// __STACK_H__