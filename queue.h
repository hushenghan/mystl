#pragma once
#include"util.h"
#include"list.h"

namespace mystl {

template<typename T> 
class queue :public list<T> {
public:
	void enqueue(T const& e) { this->insertAsLast(e); }
	T dequeue() { return this->remove(this->first()); }
	T& front() { return this->first()->data; };
};


};