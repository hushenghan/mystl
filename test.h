#include "./vector.h"
#include "./list.h"
#include "stack.h"
#include "queue.h"
#include "bintree.h"


using std::cout;
using std::endl;

void test_vector() {
	// new and delete
	int t[] = { 7, 1, 3, 2, 4, 6 };
	mystl::vector<int> t1(t, 6);	
	cout << "init: 7 1 3 2 4 6" << endl;
	t1.show();
	auto t2(t1);
	t2.show();
	cout << "_size: " << t2.size() <<endl;
	cout << "disordered: " << t2.disordered() << endl;
	cout << "vec[2]: " << t2[2] << endl;
	cout << "find(4) " << t2.find(4) << endl;
	cout << "find(5) " << t2.find(5) << endl;

	t2.remove(4);
	cout << "remove(4) ";  t2.show();

	t2.push_back(100);
	t2.push_back(100);
	cout << "pushback 100 120: "; t2.show();
	t2.remove(2,4);
	cout << "remove(2,4): "; t2.show();
	
	t2.deduplicate();
	cout << "deduplicate: "; t2.show();

	t2.sort();
	cout << "sort: "; t2.show();
}

void test_list() {
	int t[] = {5,3,1};
	mystl::vector<int> vec(t, 3);
	mystl::list<int> lst(vec);
	lst.show();

	lst.insertAsLast(8);
	lst.insertAsFirst(0);
	lst.insertAfter(lst.first(), 8);
	lst.insertBefore(lst.first()->succ->succ, 8);

	lst.show();

	cout << "max " << lst.selectMax()->data << endl;

	lst.deduplicate();
	cout << "deq "; lst.show();

	lst.sort();
	cout << "sorted: "; lst.show();

}

void test_stack() {
	mystl::stack<int> stk;
	stk.push(1);
	cout << stk.size()<<endl;
	cout << stk.pop() << endl;
	stk.push(2);
	stk.push(3);
	stk.push(4);
	stk.pop();
	stk.pop();
	cout << stk.pop() << endl;
}

void test_queue() {
	mystl::queue<int> que;
	que.enqueue(3);
	que.enqueue(5);
	que.show();
	que.dequeue();
	que.enqueue(7);
	que.show();
}

template<typename T>
struct Increase {
	virtual void operator()(T& e) { e++; }
};

template<typename T>
struct Show
{
	virtual void operator()(T& e) { cout << e <<" "; }
};

void test_bintree() {
	mystl::BinTree<int> tree;
	tree.insertAsRoot(0);
	tree.insertAsLC(tree.root(), 1);
	tree.insertAsRC(tree.root(), 2);
	tree.insertAsRC(tree.root()->lChild, 3);
	tree.insertAsRC(tree.root()->rChild, 4);
	Increase<int> inc;
	Show<int> show;
	cout << "travlevel : \n";
	tree.travLevel(show);
	cout << "\ninc: " << endl;
	tree.travLevel(inc);
	tree.travLevel(show);

	cout << "\ntravIn:\n";
	tree.travIn(show);
	cout << "\ntravPre : \n";
	tree.travPre(show);
	cout << "\ntravPost: \n";
	tree.travPost(show);
}