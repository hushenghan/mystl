#pragma once
#include "defs.h"
#include "util.h"

namespace mystl {

#define DEFALT_CAPACITY 3
typedef long Rank;

template<typename T> class vector
{
protected:
	Rank _size; int _capacity; T* _elem;
	void copyFrom(T const* A, Rank lo, Rank hi); 
	void expand();//扩容
	void shrink();//压缩
	bool bubble(Rank lo, Rank hi);
	void bubbleSort(Rank lo, Rank hi);
	Rank max(Rank lo, Rank hi);
	void selectionSort(Rank lo, Rank hi);
	void merge(Rank lo,Rank mi, Rank hi);
	void mergeSort(Rank lo, Rank hi);
	Rank partition(Rank lo, Rank hi);
	void quickSort(Rank lo, Rank hi);
	void heapSort(Rank lo, Rank hi);
	void swapByRank(Rank a, Rank b);

public:
	// construct
	vector(int c = DEFALT_CAPACITY, int s = 0, T v = 0)
	{
		if (s > c)
			exit(-1);
		_elem = new T[_capacity = c];  for (_size = 0;_size < s; _elem[_size++] = v);
	}

	vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
	vector(T const* A, Rank n) { copyFrom(A, 0, n); }
	vector(vector<T> const& v, Rank lo, Rank hi) { copyFrom(v._elem, lo, hi); }
	vector(vector<T> const& v) { copyFrom(v._elem, 0, v._size); }

	//析构
	~vector(){delete[] _elem;}

	// only read api
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	int disordered() const; // wether sorted?
	Rank find(T const& e) const { return find(e, 0, _size); }
	Rank find(T const& e, Rank lo, Rank hi) const;
	Rank search(T const& e) const { return (0 >= _size) ? -1 : search(e, 0, _size); }
	Rank search(T const& e, Rank lo, Rank hi) const;
	
	
	// read / write
	T& operator[] (Rank r) const; 
	vector<T>& operator=(vector<T> const&);
	T remove(Rank r); // remove rank k, not elem
	int remove(Rank lo, Rank hi); // delete elems in [lo, hi)
	Rank insert(Rank k, T const& e);
	Rank insert(T const& e) { return insert(_size, e); }
	Rank push_back(T const& e) { return insert(_size, e); };
	Rank pop_back() { return remove(_size - 1); };

	void sort() { sort(0, _size); }
	void sort(Rank lo, Rank hi);
	void unsort(Rank lo, Rank hi); // 置乱
	void unsort() { unsort(0, _size); }
	int deduplicate(); //无序去重e
	int uniquify(); //有序去重
	
	// visit
	void traverse(void (*visit)(T&)); //使用函数指针进行遍历, 只读或局部性修改
	template <typename VST> void traverse(VST&); //遍历， 使用函数对象，可全局性修改

	// show
	void show() {
		for (int i = 0;i < _size;i++)
			std::cout << _elem[i] << " ";
		std::cout << std::endl;
	}


};



/*
*		==========================  implement  ====================================
*/

	template<typename T>
	void vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
		// should we check wether the _elem already exist?
		_elem = new T[2 * (hi - lo)]; _size = 0;
		while (lo < hi)
			_elem[_size++] = A[lo++];
		expand();
	}

	template<typename T>
	void vector<T>::expand() {
		if (_size < _capacity) return;
		if (_capacity < DEFALT_CAPACITY) _capacity = DEFALT_CAPACITY;
		if (_capacity < _size) _capacity = _size;
		T* oldelem = _elem; _elem = new T[_capacity << 1];
		for (int i = 0;i < _size;i++)
			_elem[i] = oldelem[i];
		delete[] oldelem;
	}

	template<typename T>
	void vector<T>::shrink() {
		if (_capacity < DEFALT_CAPACITY << 1) return; // too small to shrink
		if (_size << 2 > _capacity) return; //_size > 0.25 _capacity, dont need to shrink
		T* oldelem = _elem;
		// _elem = new T[_capacity >> 1] // capacity / 2
		_elem = new T[_size << 1]; // _size * 2 

		for (int i = 0;i < _size;i++)
			_elem[i] = oldelem[i];
		delete[] oldelem;
	}

	template<typename T>
	void vector<T>::bubbleSort(Rank lo, Rank hi) {
		while (!bubble(lo, hi--));
	}

	template<typename T>
	bool vector<T>::bubble(Rank lo, Rank hi) {
		bool sorted = true;
		while (++lo < hi) {
			if (_elem[lo - 1] > _elem[lo]) {
				sorted = false;
				swapByRank(lo-1, lo);
			}
		}
		return sorted;
	}

	template <typename T>
	void vector<T>::mergeSort(Rank lo, Rank hi) {
		if (hi - lo < 2) return ; // 只有一个
		int mi = (lo + hi) >> 1;
		mergeSort(lo, mi);
		mergeSort(mi, hi);
		merge(lo, mi, hi);
	}

	template<typename T>
	void vector<T>::merge(Rank lo, Rank mi, Rank hi) {
		T* A = _elem + lo;
		int lb = mi - lo; T* B = new T[lb];
		for (Rank i = 0; i < lb; B[i] = A[i++]);
		int lc = hi - mi; T* C = _elem + mi;
		for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) {
			if ((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++]; // 保证稳定性
			if ((k < lc) && (!(j < lb) || (C[k] < B[j])))  A[i++] = C[k++];
		}
		delete[] B;
	}


	template<typename T>
	void vector<T>::swapByRank(Rank a, Rank b) {
		T tmp = _elem[a];
		_elem[a] = _elem[b];
		_elem[b] = tmp;
	}

	template<typename T>
	void vector<T>::selectionSort(Rank lo, Rank hi) {
		for (;lo < hi;hi--) {
			Rank mxR = max(lo, hi);
			swapByRank(mxR, hi-1);
		}
	}


	template<typename T>
	Rank vector<T>::partition(Rank lo, Rank hi) {

		Rank p = lo;
		T key = _elem[p];
		while (lo < hi) {
			while ((lo < hi) && (key <= _elem[--hi]));
			if (lo < hi) {
				//swapByRank(p, hi);
				_elem[lo] = _elem[hi];
				lo++;
			}

			while ((lo < hi) && (_elem[lo++] < key));
			if (lo < hi) {
				//swapByRank(p, lo);
				_elem[hi] = _elem[lo];
				hi--;
			}
			_elem[lo] = key;
			return lo;
		}

		return p;
	}


	template<typename T>
	void vector<T>::quickSort(Rank lo, Rank hi) {
		if (lo >= hi)
			return;

		Rank p = partition(lo, hi);
		quickSort(lo, p);
		quickSort(p + 1, hi);
	}

	// TODO
	template<typename T>
	void vector<T>::heapSort(Rank lo, Rank hi) {

	}



	template<typename T>
	Rank vector<T>::max(Rank lo, Rank hi) {
		Rank mxR = lo;
		while (++lo < hi) {
			if (_elem[mxR] < _elem[lo])
				mxR = lo;
		}
		return mxR;
	}




	// public


#pragma region R
	template<typename T>
	Rank vector<T>::find(T const& e, Rank lo, Rank hi) const {
		while ((lo <= --hi) && (e != _elem[hi]));
		return hi; // fail -> return lo;
	}


	template<typename T>
	int vector<T>::disordered() const {
		int n = 0;
		for (int i = 0;i < _size - 1;i++)
			if (_elem[i] > _elem[i + 1])
				n++;
		return n;
	}


	template<typename T>
	Rank vector<T>::search(T const& e, Rank lo, Rank hi) const {
		return (rand() % 2) ?
			binSearch(_elem, e, lo, hi) : fibSearch(_elem, e, lo, hi);
	}


#pragma endregion 



#pragma region R/W
	template<typename T>
	vector<T>& vector<T>::operator=(vector<T> const& vt) {
		if (_elem) delete[] _elem; //release
		copyFrom(vt._elem, 0, vt.size());
		return *this;
	}

	template<typename T>
	T& vector<T>::operator[] (Rank r) const {
		return _elem[r];
	}


	template<typename T>
	int vector<T>::remove(Rank lo, Rank hi) {
		for (int i = hi; i < _size; i++)
			_elem[lo++] = _elem[hi++];
		_size = lo; // update size
		shrink();
		return hi - lo; // return number of deleted elems
	}

	template<typename T>
	T vector<T>::remove(Rank r) {
		T e = _elem[r];
		remove(r, r + 1);
		return e;
	}

	template<typename T>
	Rank vector<T>::insert(Rank k, T const& e) {
		expand();
		for (int i = _size;i > k;i--)
			_elem[i] = _elem[i - 1];
		_elem[k] = e;
		_size++;
		return _size;
	}

	template<typename T>
	void vector<T>::unsort(Rank lo, Rank hi) {
		/*
		for (int i = hi - 1; i >= lo; i--)
			swapByRank(v[i - 1], v[(rand() % i) + lo]);
		*/


		// book
		T* v = _elem + lo;
		for (int i = hi - lo; i > 0; i--)
			swapByRank(v[i - 1], v[rand() % i]);
	}

	template<typename T>
	int vector<T>::deduplicate() {
		int oldSize = _size;
		Rank i = 1;
		while (i < _size)
			(find(_elem[i], 0, i) < 0) ?
			i++ : remove(i);
		return oldSize - _size; // decreased number
	}

	template<typename T>
	int vector<T>::uniquify() {
		int i = 0, j = 0;
		while (++j < _size) {
			if (_elem[i] != _elem[j])
				_elem[++i] = _elem[j];
		}
		_size = ++i;  shrink();
		return j - i;
	}



	template<typename T>// function pointer
	void vector<T>::traverse(void (*visit)(T&)) {
		for (int i = 0;i < _size;i++)
			visit(_elem[i]);
	}


	/*
	// 函数对象
	template<typename T> struct Increase
	{virtual void operator()(T& e) {e++;} }

	template<typename T> void increase(vector<T> & v)
	{v.traverse(Increase<T>()); }
	*/

	template<typename T>
	template<typename VST> // 利用函数对象机制的遍历
	void vector<T>::traverse(VST& visit) {
		for (int i = 0;i < _size; i++)
			visit(_elem[i]);
	}

	template <typename T>
	void vector<T>::sort(Rank lo, Rank hi) {
		int flag = rand() % 4;
		flag = 5;
		switch (flag) {
		case 1: bubbleSort(lo, hi); break;
		case 2: selectionSort(lo, hi); break;
		case 3: mergeSort(lo, hi);break;
		case 4: heapSort(lo, hi); break;
		default: quickSort(lo, hi); break;
		}
	}
#pragma endregion 

};