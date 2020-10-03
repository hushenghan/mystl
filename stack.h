#pragma once
#include "vector.h"

namespace mystl {

	template <typename T>
	class stack : public vector<T> {
	public: // size(), empty()等public接口可直接使用
		void push(T const& e) { this->insert(this->size(), e); } // 入栈： 等效于将新元素作为向量的末元素插入
		T pop() { return this->remove(this->size() - 1); } // 出栈：等效于删除向量的末元素
		T& top() { return (*this)[this->size() - 1]; } // 取顶，直接返回向量的末元素
	};


	void convert_recursion(stack<char>& S, __int64 n, int base) {
		static char digit[] = { '0', '1', '2', '3', '4', '5','6','7','8','9','A','B','C','D','E','F' };
		while (n > 0) {
			convert_recursion(S, n / base, base);
			S.push(digit[n % base]);
		}
	}

	void convert_iterate(stack<char>& S, __int64 n, int base) {
		static char digit[] = { '0', '1', '2', '3', '4', '5','6','7','8','9','A','B','C','D','E','F' };
		int remainder = (int)(n % base);
		S.push(digit[remainder]);
		n /= base;
	}


	// 括号匹配
	void trim(const char exp[], int& lo, int& hi) {
		while ((lo <= hi) && (exp[lo] != '(') && (exp[lo] != ')')) lo++; // first ( from left
		while ((lo <= hi) && (exp[hi] != '(') && (exp[hi] != ')')) hi--; // first ) from right
	}

	int divide(const char exp[], int lo, int hi) { // divide, visit from left to the poision where number of ( and ) is the same
		int mi = lo; int crc = 1;
		while ((0 < crc) && (++mi < hi)) {
			if (exp[mi] == ')') crc--;
			if (exp[mi] == '(') crc++;
		}
		return mi; // if mi<=hi, valid, else can not match
	}
	// recursion
	bool paren_recursion(const char exp[], int lo, int hi) {
		trim(exp, lo, hi); if (lo > hi) return true;
		if (exp[lo] != '(') return false;
		if (exp[hi] != ')') return false;
		int mi = divide(exp, lo, hi);
		if (mi > hi) return false; // number of ( ) not match until the end, so false
		return paren_recursion(exp, lo + 1, mi - 1) && paren_recursion(exp, mi + 1, hi);
	}

	// iterate
	bool paren_iterate(const char exp[], int lo, int hi) {
		stack<char> S;
		for (int i = 0; exp[i]; i++) {
			switch (exp[i]) {
			case '(': case '[': case '{': S.push(exp[i]); break;
			case ')': if ((S.empty()) || ('(' != S.pop())) return false; break;
			case ']': if ((S.empty()) || (']' != S.pop())) return false; break;
			case '}': if ((S.empty()) || ('}' != S.pop())) return false; break;
			default: break;
			}
			return S.empty();
		}
		return false;
	}


	// 求值，
#define N_OPTR 9 //运算符总数
	typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //运算符集合
	//加、减、乘、除、乘斱、阶乘、左括号、右括号、起始符不终止符
	const char pri[N_OPTR][N_OPTR] = { //运算符优先等级 [栈顶] [弼前] 6 /* |-------------- 弼前运算符 --------------| */
	/*				+	 -	   *    /    ^    !    (    )   \0   */
		/* --	+ */	'>', '>', '<', '<', '<', '<', '<', '>', '>',
		/* |	- */	'>', '>', '<', '<', '<', '<', '<', '>', '>',
		/* 栈	* */	'>', '>', '>', '>', '<', '<', '<', '>', '>',
		/* 顶	/ */	'>', '>', '>', '>', '<', '<', '<', '>', '>',
		/* 运	^ */	'>', '>', '>', '>', '>', '<', '<', '>', '>',
		/* 算	! */	'>', '>', '>', '>', '>', '>', ' ', '>', '>',
		/* 符	( */	'<', '<', '<', '<', '<', '<', '<', '=', ' ',
		/* |	) */	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		/* --	\0 */	'<', '<', '<', '<', '<', '<', '<', ' ', '='
	};

	/*
	float evaluate(char* S, char*& RPN) {
		stack<float> opnd; stack<char> optr;
		optr.push('\0');
		while (!optr.empty()) {
			if (isdigit(*S)) {
				readNumber(S, opnd); append(RPN, opnd.top()); // 读取操作数， 并将其接至RPN末尾
			}
			else {
				switch (orderBetween(optr.top(), *S)) {
				case '<': //栈顶运算符优先级更低时
					optr.push(*S); S++;
					break;
				case '=': // 优先级相等，为右括号或者尾哨兵'\0’
					optr.pop(); S++;
					break;
				case '>':// 栈顶运算符优先级更高，进行运算，并将结果重新入栈
					char op = optr.pop();
					append(RPN, op);
					if ('!' == op) { // 若属于一元运算符
						float pOpnd = opnd.pop();
						opnd.push(calcu(op.pOpnd));
					}
					else { // 二元运算符
						float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //取出后前操作数
						opnd.push(calcu(pOpnd1, op, pOpnd2)); // 实施二元计算，结果入栈
					}
					break;

				default: exit(-1); // 语法错误
				}//switch

			}
		}
		return opnd.pop(); // 最终的结果
	}

	*/


	// 八皇后问题
	struct Queen {
		int x, y;
		Queen(int xx = 0, int yy = 0) :x(xx), y(yy) {};
		bool operator==(Queen const& q) const {
			return (x == q.x) ||
				(y == q.y) ||
				(x + y == q.x + q.y) ||
				(x - y == q.x - q.y);
		}
		bool operator != (Queen const& q) const { return !(*this == q); }
	};

	void placeQueens(int N) {
		stack<Queen> solu;
		Queen q(0, 0);
		do {
			if (N <= solu.size() || N <= q.y) { // 若已经出界， 则
				q = solu.pop(); q.y++; //回溯一行，并继续试探下一行
			}
			else {
				while ((q.y < N) && (0 <= solu.find(q))) {
					q.y++; // nCheck++; //尝试找到可摆放下一皇后的列
				}
				if (N > q.y) { // 若存在可摆放的列，则
					solu.push(q);
					//if (N <= solu.size()) nSolu++; // 若部分解已经成为全局解，则通过全局变量nSolu计数
					q.x++; q.y = 0; //转入下一行，从第0列开始，试探下一皇后
				}
			}
		} while ((0 < q.x) || (q.y < N)); // 所有分支均已经穷举或剪枝之后，算法结束。
	}


	// 迷宫
	typedef enum { AVAILABLE, ROUTE, BACKTRACKED, WALL } Status; // 迷宫单元状态
	// 原始可用的， 在当前路径上的，所有方向均尝试失败后回溯的，不可使用的

	typedef enum { UNKNOWN, EAST, SOUTH, WEST, NORTH, NO_WAY } ESWN; // 单元的相对邻接方向
	// 未定， 东南西北 无路

	inline ESWN nextESWN(ESWN eswn) { return ESWN(eswn + 1); }// 依次转向下一个邻接方向

	struct Cell {
		int x, y; Status status;
		ESWN incoming, outgoing; //进入，走出方向
	};

	#define LABY_MAX 24 //最大迷宫尺寸
	Cell laby[LABY_MAX][LABY_MAX]; //迷宫

	// 邻格查询
	inline Cell* neighbor(Cell* cell) { // 查询当前位置的相邻格点
		switch (cell->outgoing) {
		case EAST: return cell + LABY_MAX; // 向东
		case SOUTH: return cell + 1; // 南
		case WEST: return cell - LABY_MAX; // 西
		case NORTH: return cell - 1; //北
		default:exit(-1);
		}
	}

	// 邻格转入
	inline Cell* advance(Cell* cell) { // 从当前位置转入相邻格点
		Cell* next;
		switch (cell->outgoing) {
		case EAST: next = cell + LABY_MAX;	next->incoming = WEST; break; //向东
		case SOUTH: next = cell + 1;		next->incoming = NORTH; break;
		case WEST: next = cell - LABY_MAX;	next->incoming = EAST; break;
		case NORTH:next = cell - 1; next->incoming = SOUTH; break;
		default: exit(-1);
		}
		return next;
	}

	// 
	bool labyrinth(Cell Laby[LABY_MAX][LABY_MAX], Cell* s, Cell* t) {
		if ((AVAILABLE != s->status) || (AVAILABLE != t->status)) return false; // 退化情况
		stack<Cell*> path; // 用栈记录通路
		s->incoming = UNKNOWN; s->status = ROUTE; path.push(s); //起点
		do { // 从起点出发不断试探、回溯
			Cell* c = path.pop();
			if (c == t) return true; 
			while (NO_WAY > (c->outgoing = nextESWN(c->outgoing))) //逐一检查所有方向
				if (AVAILABLE == neighbor(c)->status) break; //试图找到尚未试探的方向
			if (NO_WAY <= c->outgoing) { //若所有方向都已经尝试过
				c->status = BACKTRACKED; c = path.pop();
			}
			else {
				path.push(c = advance(c)); c->outgoing = UNKNOWN; c->status = ROUTE;
			}
		} while (!path.empty());
		return false;

}



};
