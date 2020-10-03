#pragma once
#include <stdlib.h>
#include <iostream>
#include "stack.h"
#include <stdio.h>


template <typename T> static bool lt(T* a, T* b) { return lt(*a, *b); } //less than
template <typename T> static bool lt(T& a, T& b) { return a < b; } //less than
template <typename T> static bool eq(T* a, T* b) { return eq(*a, *b); } //equal
template <typename T> static bool eq(T& a, T& b) { return a == b; } //equal

template<typename T> static T max(T& a, T& b) { return a > b ? a : b; }
template<typename T> static T max(T a, T b) { return a > b ? a : b; }

// ======= clean ======
template<typename T> struct Cleaner {
	static void clean(T x) {
#ifdef _DEBUG
		static int n = 0;
		if (7 > strlen(typeid(T).name())) {//∫ˆ¬‘∏¥‘”¿‡–Õ
			printf("\t<%s>[%d]=", typeid(T).name(), ++n);
			//print(x);
			printf("purged \n");
		}
#endif
	}
};


template<typename T> struct Cleaner<T*> {
	static void clean(T* x) {
		if (x) { delete x; }
#ifdef _DEBUG
		static int n = 0;
		printf("\t<%s>[%d] released\n", typeid(T*).name(), ++n);
#endif
	}
};

template<typename T> void release(T x) { Cleaner<T>::clean(x); }



