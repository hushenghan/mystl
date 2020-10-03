#pragma once

#include"queue.h"
#include"stack.h"
#include"binnode.h"
#include"util.h"

namespace mystl {

	template<typename T>
	class BinTree {
	protected:
		int _size;
		BinNodePosi(T) _root;
		virtual int updateHeight(BinNodePosi(T) x);
		void updateHeightAbove(BinNodePosi(T) x);
	public:
		BinTree():_size(0), _root(nullptr) {};
		~BinTree() { if (0 < _size) remove(_root); }
		int size() const { return _size; }
		bool empty() const { return !_root; }
		int remove(BinNodePosi(T) x);
		BinNodePosi(T) root() const { return _root; }
		BinNodePosi(T) insertAsRoot(T const& e);
		BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); // e作为x的左孩子插入(认为x原来没有左孩子)
		BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);
		BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*& T); // T 作为x的左子树接入
		BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*& T);
		BinTree<T>* secede(BinNodePosi(T) x); //将子树从当前树中摘除，并将其转换为一颗独立子树

		template<typename VST>
		void travLevel(VST& visit) { if (_root) _root->travLevel(visit); } //先序遍历
		template<typename VST>
		void travPre(VST& visit) { if (_root) _root->travPre(visit); } // 中序遍历
		template<typename VST>
		void travIn(VST& visit) { if (_root) _root ->travIn(visit); }
		template<typename VST>
		void travPost(VST& visit) { if (_root) _root->travPost(visit); } // 后序遍历

		// 比较器
		bool operator<  (BinTree<T>const& t) { return _root && t._root && lt(_root, t._root); }
		bool operator== (BinTree<T>const& t) { return _root && t._root && (_root == t._root); }

	};



	template<typename T>
	int BinTree<T>::updateHeight(BinNodePosi(T) x) {
		return x->height = 1 + max(stature(x->lChild), stature(x->rChild));
	}

	template<typename T>
	void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) {
		while (x) {
			updateHeight(x);
			x = x->parent;
		}
	}

	template<typename T>
	BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e) {
		_size = 1;
		return _root = new BinNode<T>(e);
	}

	template<typename T>
	BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e) {
		_size++;
		x->insertAsLC(e);
		updateHeightAbove(x);
		return x->lChild;
	}

	template<typename T>
	BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e) {
		_size++;
		x->insertAsRC(e);
		updateHeightAbove(x);
		return x->rChild;
	}


	// 子树接入
	template<typename T>
	BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>*& S) {
		if (x->lChild = S->_root) x->lChild->parent = x; // 接入
		_size += S->_size; updateHeightAbove(x); //更新规模和所有祖先的高度
		S->_root = nullptr; S->_size = 0; release(S); S = nullptr; return x;
	}

	template<typename T>
	BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>*& S) {
		if (x->rChild = S->_root) x->rChild->parent = x;
		_size += S->_size; updateHeightAbove(x);
		S->_root = nullptr; S->_size = 0; release(S); S = nullptr; return x;
	}

	//子树删除
	template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
	int BinTree<T>::remove(BinNodePosi(T) x) { //assert: x为二叉树中的合法位置
		FromParentTo(*x) = nullptr; //切断来自父节点的指针
		updateHeightAbove(x->parent); //更新祖先高度
		int n = removeAt(x); _size -= n; return n; //删除子树x，更新规模，返回删除节点总数
	}

	template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
	static int removeAt(BinNodePosi(T) x) { //assert: x为二叉树中的合法位置
		if (!x) return 0; //递归基：空树
		int n = 1 + removeAt(x->lChild) + removeAt(x->rChild); //递归释放左、右子树
		release(x->data); release(x); return n; //释放被摘除节点，并返回删除节点总数
	}

	// 子树分离
	template<typename T> // 二叉树子树分离算法： 将子树x从当前树摘除，并将其封装为一颗独立子树
	BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {
		FromParentTo(*x) = nullptr;
		updateHeightAbove(x->parent);
		BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = nullptr; // 新树以x为根
		S->_size = x->size(); _size -= S->_size; return S;
	}


};