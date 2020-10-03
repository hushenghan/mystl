#pragma once
#include"stack.h"
#include"queue.h"

namespace mystl {

#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p) ? (p)->height:-1) // 节点高度
	typedef enum { RB_RED, RB_BLACK } RBColor; // 节点颜色

	// 状态和性质的判断
#define IsRoot(x)		(!((x).parent))
#define IsLChild(x)		(!IsRoot(x) && (&(x) == (x).parent->lChild))
#define IsRChild(x)		(!IsRoot(x) && (&(x) == (x).parent->rChild))
#define HasParent(x)	(!IsRoot(x))
#define HasLChild(x)	((x).lChild)
#define HasRChild(x)	((x).rChild)
#define HasChild(x)		(HasLChild(x) || HasRChild(x))
#define HasBothChild(x)	(HasLChild(x) && HasRChild(x))
#define IsLeaf(x)		(!HasChild(x))

// 与此节点相关的节点以及指针
#define sibling(p) (			\
	IsLChild(*(p)) ?			\
		(p)->parent->rChild :	\
		(p)->parent->lChild		\
) //兄弟

#define uncle(p) (					\
	IsLChild(*((p)->parent)) ?		\
		(p)->parent->parent->rChild \
		(p)->parent->parent->lChild \
) // 叔叔= 父亲的兄弟

#define FromParentTo(x) (	\
	IsRoot(x) ? _root : (	\
	IsLChild(x) ? (x).parent->lChild : (x).parent->rChild \
	) \
) //来自父亲的指针

	// 二叉树节点模板类
	template<typename T> struct BinNode { 
		T data;
		BinNodePosi(T) parent; BinNodePosi(T) lChild; BinNodePosi(T) rChild;
		int height;
		int npl; // null path length 
		RBColor color;

		// 构造函数
		BinNode() : parent(nullptr), lChild(nullptr), rChild(nullptr), height(0), npl(1), color(RB_RED) {};
		BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED)
			:data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c) {}

		// 操作
		int size(); //当前节点的后代总数，子树的规模
		BinNodePosi(T) insertAsLC(T const&); // 插入到左孩子
		BinNodePosi(T) insertAsRC(T const&);
		BinNodePosi(T) succ(); //当前节点的中序遍历的直接后继

		
		template<typename VST> void travLevel(VST&); //子树层次遍历
		template<typename VST> void travPre(VST&); //子树先序遍历
		template<typename VST> void travIn(VST&); //子树中序遍历
		template<typename VST> void travPost(VST&); //子树后序遍历
		
	// 比较器， 判别器
		bool operator<(BinNode const& bn) { return data < bn.data; }
		bool operator==(BinNode const& bn) { return data == bn.data; }

	private:
		template<typename VST> void travPre_R(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travIn_R(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travPost_R(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travPre_I1(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travPre_I2(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travIn_I1(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travIn_I2(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travIn_I3(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travIn_I4(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travPost_I(BinNodePosi(T) x, VST& visit);

	};


	/*
					IMPLEMENT
	*/
	
	template<typename T>
	int BinNode<T>::size() {
		if (!this) return 0;
		int count = 0;
		queue<BinNodePosi(T)> Q;
		Q.enqueue(this);
		while (!Q.empty()) {
			BinNodePosi(T) x = Q.dequeue();	count++;
			if (HasLChild(*x)) Q.enqueue(x->lChild);
			if (HasRChild(*x)) Q.enqueue(x->rChild);
		}
		return count;
	}




	// 插入孩子节点
	template<typename T>
	BinNodePosi(T) BinNode<T>::insertAsLC(T const& e) { return lChild = new BinNode(e, this); }

	template<typename T>
	BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) { return rChild = new BinNode(e, this); }

	// 中序遍历的直接后继: 若有右孩子，则为右子树的最左侧左孩子，否则(没有右孩子)，则回溯到父节点(如果此父节点仍然是右孩子，说明已经访问过，继续回溯，直到为左孩子)
	template<typename T>
	BinNodePosi(T) BinNode<T>::succ() {
		BinNodePosi(T) s = this;
		if (rChild) {
			s = rChild;
			while (HasLChild(*s)) s = s->lChild;
		}
		else {
			while (IsRChild(*s)) s = s->parent;
			s = s->parent; // 最后再右上方移动一步，抵达直接后继
		}
		return s;
	}


//遍历

//先序    
	template<typename T>
	template<typename VST> 
	void BinNode<T>::travPre(VST&visit) {
		switch (rand() % 3) {
		case 1: travPre_I1(this, visit); break;
		case 2: travPre_I2(this, visit); break;
		default: travPre_R(this, visit); break;
		}		
	}


		// recursion
	template<typename T>
	template<typename VST>
	void BinNode<T>::travPre_R(BinNodePosi(T) x, VST& visit) {
		if (!x) return;
		visit(x->data);
		travPre_R(x->lChild, visit);
		travPre_R(x->rChild, visit);
	}

		// version 1
	template<typename T>
	template<typename VST>
	void  BinNode<T>::travPre_I1(BinNodePosi(T) x, VST& visit) {
		stack <BinNodePosi(T)> S;
		if (x) S.push(x);
		while (!S.empty()) {
			x = S.pop(); visit(x->data);
			if (HasRChild(*x)) S.push(x->rChild); //先压入右侧，先入后出
			if (HasLChild(*x)) S.push(x->lChild);
		}
	}

		// version 2
	template<typename T, typename VST>
	static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, stack <BinNodePosi(T)>& S) {
		while (x) {
			visit(x->data);
			S.push(x->rChild);
			x = x->lChild;
		}
	}

	template<typename T>
	template<typename VST>
	void BinNode<T>::travPre_I2(BinNodePosi(T) x, VST& visit) {
		stack<BinNodePosi(T)> S;
		while (true) {
			visitAlongLeftBranch(x, visit, S);
			if (S.empty()) break;
			x = S.pop();
		}
	}



// 中序
	template<typename T> 
	template <typename VST>
	void BinNode<T> ::travIn(VST& visit) {
		switch (rand() % 5) {
		case 1: travIn_I1(this, visit); break;
		case 2: travIn_I2(this, visit); break;
		case 3: travIn_I3(this, visit); break;
		case 4: travIn_I4(this, visit); break;
		default: travIn_R(this, visit); break;
		}
	}

	//  recursion 
	template<typename T>
	template<typename VST>
	void BinNode<T>::travIn_R(BinNodePosi(T) x, VST& visit) {
		if (!x) return;
		travIn_R(x->lChild, visit);
		visit(x->data);
		travIn_R(x->rChild, visit);
	}

	// version1
	template<typename T>
	static void goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)>& S) {
		while (x) {
			S.push(x); x = x->lChild;
		}
	}

	template<typename T>
	template<typename VST>
	void BinNode<T>::travIn_I1(BinNodePosi(T) x, VST& visit) {
		stack<BinNodePosi(T)> S;
		while (true) {
			goAlongLeftBranch(x, S);
			if (S.empty()) break;
			x = S.pop();
			visit(x->data);
			x = x->rChild;
		}
	}


	// version 2
	template<typename T>
	template<typename VST>
	void BinNode<T>::travIn_I2(BinNodePosi(T) x, VST& visit) {
		stack<BinNodePosi(T)> S;
		while (true) {
			if (x) {
				S.push(x);
				x = x->lChild;
			}
			else if (!S.empty()) {
				x = S.pop();
				visit(x->data);
				x = x->rChild;
			}
			else
				break;
		}
	}

	// version 3 无辅助栈
	template<typename T>
	template<typename VST>
	void BinNode<T>::travIn_I3(BinNodePosi(T) x, VST& visit) {
		bool backtrack = false; //前一步是否从右子树回溯，省去栈
		while (true)
			if (!backtrack && HasLChild(*x)) //若有左子树且不是刚刚回溯，则
				x = x->lChild;
			else { //否则，无左子树或刚刚回溯（相当于无左子树）
				visit(x->data);
				if (HasRChild(*x)) {
					x = x->rChild; //深入右子树继续遍历
					backtrack = false; // 关闭回溯标志
				}
				else {// 若右子树为空，则
					if (!(x = x->succ())) break; //回溯(含抵达末节点时的退出返回)
					backtrack = true; //并设置回溯标志
				}
			}
	}

	//version 4, 无辅助栈，无标志位
	template<typename T>
	template<typename VST>
	void BinNode<T>::travIn_I4(BinNodePosi(T) x, VST& visit) {
		while (true) {
			if (HasLChild(*x))
				x = x->lChild;
			else {
				visit(x->data);
				while (!HasRChild(*x))
					if (!(x = x->succ())) return;
					else visit(x->data);
				x = x->rChild;
			}
		}
	}




// 后序

	template<typename T>
	template<typename VST> 
	void BinNode<T>::travPost(VST& visit) {
		switch (rand() % 2) {
		case 1: travPost_I(this, visit);
		default: travPost_R(this, visit);
		}
	}


	// recursion
	template<typename T>
	template<typename VST>
	void BinNode<T>::travPost_R(BinNodePosi(T) x, VST& visit) {
		if (!x) return;
		travPost_R(x->lChild, visit);
		travPost_R(x->rChild, visit);
		visit(x->data);
	}

	// 迭代版后续遍历, 尽可能往左，实在没左节点的话，再向右
	template<typename T> // 在以S栈顶节点为根的子树中，找到最高左侧可见叶节点
	static void gotoHLVFL(stack <BinNodePosi(T)>& S) {  // 沿途所遇节点依次入栈
		while (BinNodePosi(T) x = S.top()) {
			if (HasLChild(*x)) {//尽可能往左
				if (HasRChild(*x)) S.push(x->rChild); // 若有右孩子，优先入栈
				S.push(x->lChild); //然后才转至左孩子
			}
			else
				S.push(x->rChild);
		}
		S.pop();
	}

	
	template<typename T>
	template<typename VST>
	void BinNode<T>::travPost_I(BinNodePosi(T) x, VST& visit) {
		stack<BinNodePosi(T)> S; //辅助栈
		if (x) S.push(x);
		while (!S.empty()) {
			if (S.top() != x->parent)//若栈顶非当前节点之父(则必为其右兄)， 此时需要
				gotoHLVFL(S); // 在以其右兄为根的子树中，找到HLVFL
			x = S.pop(); visit(x->data); //弹出栈顶(即前一个节点的后继)， 并访问之
		}
	}


// 层次遍历 level
	template<typename T>
	template<typename VST>
	void BinNode<T>::travLevel(VST& visit) {//层次遍历算法
		queue<BinNodePosi(T)> Q;
		Q.enqueue(this);
		while (!Q.empty()) {
			BinNodePosi(T) x = Q.dequeue(); visit(x->data);
			if (HasLChild(*x)) Q.enqueue(x->lChild);
			if (HasRChild(*x)) Q.enqueue(x->rChild);
		}
	}
};