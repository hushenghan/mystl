#pragma once
#include "util.h"
#include "vector.h"

namespace mystl {

#define ListNodePosi(T) ListNode<T>* //
typedef long Rank;

template <typename T> 
struct ListNode {
	T data;
	ListNodePosi(T) pred; //
	ListNodePosi(T) succ;
	ListNode() : data(0), pred(nullptr), succ(nullptr) {};
	ListNode(T e, ListNodePosi(T) p = nullptr, ListNodePosi(T) s = nullptr) :
		data(e), pred(p), succ(s) {}

	ListNodePosi(T) insertAsPred(T const& e) {
		ListNodePosi(T) newNode = new ListNode<T> (e, pred, this);
		pred->succ = newNode;
		pred = newNode;	
		return newNode;
	}

	ListNodePosi(T) insertAsSucc(T const& e) {
		ListNodePosi(T) newNode = new ListNode<T> (e, this, succ);
		succ->pred = newNode;
		succ = newNode;
		return newNode;
	}

};

template<typename T> 
class list
{
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer;

protected:
	void init();
	int clear();
	void copyNodes(ListNodePosi(T) p, int n);
	void merge(ListNodePosi(T)& p1, int n1, list<T>& list2, ListNodePosi(T)& p2, int n2); //
	void mergeSort(ListNodePosi(T) p, int n); //
	void selectSort(ListNodePosi(T) p , int n);
	void insertionSort(ListNodePosi(T) p, int n);

public:
	list() { init(); }
	list(list<T> const& L);
	list(list<T> const& L, Rank k, int n);
	list(ListNodePosi(T) p, int n); 
	//list(vector<T> const& vec, Rank lo, Rank hi);
	list(vector<T> const& vec);

	~list();

// R 
	Rank size() const { return _size; }
	bool empty() const { return _size <= 0; }
	T& operator[](Rank k) const;
	ListNodePosi(T) first() const  { return header->succ; } // 
	ListNodePosi(T) last() const   { return trailer->pred; }
	bool valid(ListNodePosi(T) p)  {return p && (trailer != p) && (header != p); }
	int disordered() const;
	ListNodePosi(T) find(T const& e) const { return find(e, _size, trailer); }
	ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const; //无序区间查找
	ListNodePosi(T) search(T const& e) const {return search(e, _size, trailer);}
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);
	ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); }

//R W
	ListNodePosi(T) insertAsFirst(T const& e);
	ListNodePosi(T) insertAsLast(T const& e);
	ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e);
	ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e);
	T remove(ListNodePosi(T) p);
	void merge(list<T>& L) { merge(first(), size, L, L.first(), L._size);  }
	void sort(ListNodePosi(T) p, int n);
	void sort() { sort(first(), _size); }
	int deduplicate();
	int uniquify();
	void reverse();

// visit
	void traverse(void (*visit)(T&));
	template <typename VST>
	void traverse(VST&);
	void show();
};



/*
*	================================= implement ============================
*/

template<typename T>
void list<T>::init() {
	header = new ListNode<T>;
	trailer = new ListNode<T>;
	header->succ = trailer; header->pred = nullptr;
	trailer->pred = header; trailer->succ = nullptr;
	_size = 0;
}

template<typename T>
int list<T>::clear() {
	int oldSize = _size;
	while (0 < _size)
		remove(header->succ);
	return oldSize;
}

template<typename T>
void list<T>::copyNodes(ListNodePosi(T) p, int n) {
	init();
	while (n--) {
		insertAsLast(p.data);
		p = p->succ;
	}
}

template<typename T>
void list<T>::merge(ListNodePosi(T)& p, int n, list<T>& L, ListNodePosi(T)& q, int m) {
	ListNodePosi(T) pp = p->pred;
	while(0<m)
		if ((0 < n) && (p->data <= q->data)) {
			if (q == (p = p->succ)) break; n--;
		}
		else {
			insertBefore(p, L.remove((q = q->succ)->pred)); m--;
		}
	p = pp->succ;
}

template<typename T>
void list<T>::mergeSort(ListNodePosi(T) p, int n) {
	if (n < 2) return;
	int m = n >> 1;
	ListNodePosi(T) q = p;
	for (int i = 0;i < m;i++) q = q->succ;
	mergeSort(p, m);
	mergeSort(q, n - m);
	q = p;
	for (int i = 0;i < m;i++) q = q->succ;
	merge(p, m, *this, q, n - m);
}

template<typename T>
void list<T>::selectSort(ListNodePosi(T) p, int n) {
	ListNodePosi(T) head = p->pred, * tail = p;
	for (int i = 0;i < n;i++) tail = tail->succ;
	while (1 < n) {
		ListNodePosi(T) mx = selectMax(head->succ, n--);
		insertBefore(tail, remove(mx)); // too much cost? so many times delete and new
		tail = tail->pred;
	}
}


template<typename T>//sort for n elems that begin at p 
void list<T>::insertionSort(ListNodePosi(T) p, int n) {
	for (int r = 0;r < n;r++) {// visit each not
		insertAfter(search(p->data, r, p), p->data); // get right pos to insert?
		p = p->succ; remove(p->pred); // next node 
	}
}

template<typename T>
list<T>::list(list<T> const& L) {
	copyNodes(L.first(), L._size);
}

template<typename T>
list<T>::list(list<T> const& L, Rank r, int n) {
	copyNodes(L[r], n);
}

template<typename T>
list<T>::list(ListNodePosi(T) p, int n) {
	copyNodes(p, n);
}

template<typename T>
list<T>::list(vector<T> const& vec) {
	init();
	Rank lo = 0; Rank hi =vec.size();
	while (lo < hi) {
		insertAsLast(vec[lo]);
		lo++;
	}
}



template<typename T>
list<T>::~list() {
	clear();
	delete header;
	delete trailer;
}


#pragma region R

template<typename T>
T& list<T>::operator[](Rank r) const {
	ListNodePosi(T) p = first();
	while (0 < r--) p = p->succ;
	return p->data;
}

template<typename T>
int list<T>::disordered() const {
	int count = 0;
	ListNodePosi(T) cur = header->succ;
	while (cur != trailer.pred->pred) {
		if (cur->data > cur->succ.data)
			count++;
	}
	return count;
}



template < typename T> //在无序列表内节点p（可能是trailer）的n个（真）前驱中，找到等亍e的最后者
ListNodePosi(T) list<T>::find(T const& e, int n, ListNodePosi(T) p) const { //0<=n<=rank(p)<_size
	 while (0 < n--) //对亍p的最近的n个前驱，从右向左
		 if (e == (p = p->pred)->data) return p; //逐个比对，直至命中戒范围越界
	 return nullptr; //p越出左边界意味着区间内不含e，查找失败
} //失败时，返回NULL

template<typename T> // search data e at [p-n, p)
ListNodePosi(T) list<T>::search(T const& e, int n, ListNodePosi(T) p) const {
	while (0 < n--)  // for n preds of p, visit each one from right to left
		if (e >= (p = p->pred)->data) break; // until hit or at end
	return p; //search end
}// if fails, return leftNode.pred（maybe header）

template<typename T>
ListNodePosi(T) list<T>::selectMax(ListNodePosi(T) p, int n) {
	ListNodePosi(T) mxNode = p;
	while (n--) {
		mxNode = (mxNode->data > p->data) ? mxNode : p;
		p = p->succ;
	}
	return mxNode;
}


#pragma endregion R


#pragma region R/W
template<typename T>
ListNodePosi(T) list<T>::insertAsFirst(T const& e) {
	_size++;
	return header->insertAsSucc(e);
}

template<typename T>
ListNodePosi(T) list<T>::insertAsLast(T const& e) {
	_size++;
	return	trailer->insertAsPred(e);
}

template<typename T>
ListNodePosi(T) list<T>::insertBefore(ListNodePosi(T) p, T const& e) {
	_size++;
	return p->insertAsPred(e);
}

template<typename T>
ListNodePosi(T) list<T>::insertAfter(ListNodePosi(T) p, T const& e) {
	_size++;
	return p->insertAsSucc(e);
}

template<typename T>
T list<T>::remove(ListNodePosi(T) p) {
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	T e = p->data;
	delete p; _size--;
	return e;
}

template<typename T>
void list<T>::sort(ListNodePosi(T) p, int n) {
	int flag = 3; // rand() %3;
	switch (flag) {
		case 1: insertionSort(p, n);break;
		case 2: selectSort(p, n); break;
		default:mergeSort(p, n); break;
	}
}



template<typename T>
int list<T>::deduplicate() {
	if (_size < 2) return 0;
	int oldSize = _size;
	ListNodePosi(T) p = header;
	Rank r = 0;
	while (trailer != (p = p->succ)) {
		ListNodePosi(T) q = find(p->data, r, p);
		q ? remove(q) : r++;
	}
	return oldSize - _size;
}



template<typename T>
int list<T>::uniquify() {
	return 0;
}

template<typename T>
void list<T>::reverse() {
	ListNodePosi(T) cur = first(), next;
	header->pred = cur;
	header->succ = nullptr;

	while (cur != trailer->succ) {
		next = cur->succ;
		cur->succ = cur->pred;
		cur->pred = next;
		cur = next;
	}
}

#pragma endregion R/W


template<typename T>
void list<T>::traverse(void (*visit)(T&)) {
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) 
		visit(p->data);
}

template <typename T>
template <typename VST>
void list<T>::traverse(VST& visit) {
	for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ)
		visit(p->data);
}

template<typename T>
void list<T>::show() {
	ListNodePosi(T) p = header;
	while ((p = p->succ) != trailer)
		std::cout << p->data << " ";
	std::cout << std::endl;
}

};