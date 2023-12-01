#ifndef __STACK_H__
#define __STACK_H__

#include <iostream>
#include <algorithm>

#include <vector>

template <typename DataType>
class stack : private std::vector<DataType>
{
	public:
		
		void push(const DataType & x) {
			std::vector<DataType>::push_back(x);
		}
		
		void pop(void) {
			std::vector<DataType>::pop_back();
		}
		
		const DataType & top(void) const {
			return std::vector<DataType>::back();
		}
		
		bool empty(void) const {
			return std::vector<DataType>::empty();
		}
		
		size_t size() const {
			return std::vector<DataType>::size();
		}
};

#endif