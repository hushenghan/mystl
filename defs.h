#pragma once

template<typename T>
bool swap(T& a, T& b) {
	T c = a;
	a = b;
	b = c;
	return true;
}