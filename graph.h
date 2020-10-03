#pragma once
#include"vector.h"
#include"stack.h"
#include "queue.h"


namespace mystl {

	typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;		//顶点状态
	typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EStatus; //边状态

	template<typename Tv, typename Te> // node, edge
	class Graph {
	private:
		void reset() {
			for (int i = 0; i < n;i++) {
				status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1; // 状态，时间标签
				parent(i) = -1; priority(i) = INT_MAX; // 在遍历树种的父节点，优先级数
				for (int j = 0;j < n;j++) { // 所有边的
					if (exists(i, j)) status(i, j) = UNDETERMINED; //状态
				}
			}
		}

		void BFS(int, int&); // (连通域)BFS
		void DFS(int, int&);
		void BCC(int, int&, stack<int>&); //连通域基于DFS的双连通域分量分解算法
		bool TSort(int, int&, stack<Tv>*); //连通域基于DFS的拓扑排序算法
		template<typename PU>
		void PFS(int, PU);//连通域 优先级搜索框架
	public:
		//node
		int n;//顶点总数
		virtual int insert(Tv const&) = 0; //插入顶点，返回编号
		virtual Tv remove(int) = 0;//删除顶点以及其关联边，返回该顶点信息
		virtual Tv& vertex(int) = 0; //顶点v的数据
		virtual int inDegree(int) = 0; //入度
		virtual int outDegree(int) = 0; //出度
		virtual int firstNbr(int) = 0; //顶点v的首个邻接顶点
		virtual int nextNbr(int, int) = 0;//顶点v的相对于顶点j的下一邻接顶点
		virtual VStatus& status(int) = 0; // 顶点状态
		virtual int& dTime(int) = 0; //时间标签dTime   发现时间discovery time
		virtual int& fTime(int) = 0; // 时间标签fTime  结束时间finished  time
		virtual int& parent(int) = 0; //在遍历树中的父亲
		virtual int& priority(int) = 0; //优先级数
		//edge, 这里无向边转化为方向互逆的一对有向边
		int e; // 边总数
		virtual bool exists(int, int) = 0; //e(u,v)是否存在
		virtual void insert(Te const&, int, int, int) = 0; //在v u之间插入权重为w的边
		virtual Te remove(int, int) = 0; //删除uv间的边，返回该信息
		virtual EStatus& status(int, int) = 0; // 边的状态
		virtual Te& edge(int, int) = 0; //边uv的数据
		virtual int& weight(int, int) = 0; 
		//算法
		void bfs(int);
		void dfs(int);
		
		void bcc(int);
		stack<Tv>* tSort(int);  //基于dfs的拓扑排序算法
		void prim(int); //最小支撑树prim
		void dijkstra(int); 
		template<typename PU> void pfs(int, PU); //优先级搜索框架
	};


	// 顶点对象
	template<typename Tv>
	struct Vertex {
		Tv data; 
		int inDegree, outDegree;
		VStatus status;
		int dTime, fTime;
		int parent, priority;
		Vertex(Tv const& d= (Tv)0):
			data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
			dTime(-1), fTime(-1), parent(-1), priority(INT_MAX){}
	};

	// 边对象
	template<typename Te>
	struct Edge {
		Te data; int weight; EStatus status;
		Edge(Te const& d, int w) :data(d), weight(w), Status(UNDETERMINED) {};
	};



	template<typename Tv, typename Te>
	class GraphMatrix :public Graph<Tv, Te> {
	private:
		vector<Vertex<Tv>> V;
		vector<vector<Edge<Te>*>> E; // 边， 邻接矩阵
	public:
		GraphMatrix() { n = e = 0; }
		~GraphMatrix() {
			for (int j = 0;j < n;j++)
				for (int k = 0;k < n;k++)
					delete E[j][k];
		}
		// 顶点基本操作
		virtual Tv& vertex(int i)		{ return V[i].data; }
		virtual int inDegree(int i)		{ return V[i].inDegree;}
		virtual int outDegree(int i)	{ return V[i].outDegree;}
		virtual int firstNbr(int i) { return nestNbr(i, n); }
		virtual int nextNbr(int i, int j) // 相对于顶点j的下一邻接顶点 ?
		{
			while ((-1 < j) && (!exists(i, --j))); return j;
		}
		virtual int& dTime(int i) { return V[i].dTime; }
		virtual int& fTime(int i) { return V[i].fTime; }
		virtual int& parent(int i) { return V[i].parent; }
		virtual int& priority(int i) { return V[i].priority; }
		
		//顶点的动态操作
		virtual int insert(Tv const& vertex) { //插入顶点，返回编号
			for (int j = 0; j < n; j++) E[j].insert(nullptr); n++; //各顶点预留一条潜在的关联边
			E.insert(vector<Edge<Te>*>(n, n, (Edge<Te>*)nullptr)); //创建新顶点对象的边向量
			return  V.insert(Vertex<Tv>(vertex)); //顶点向量增加一个顶点
		}

		virtual Tv remove(int i) { // 删除第i个顶点及关联边
			// 删除
			for(int j=0; j<n;j++)  //所有入边
				if (exists(i, j)) { delete E[i][j]; V[j].inDegree--; }
			E.remove(i); n--; //删除第i行
			for(int j=0; j<n;j++) //所有出边
				if (exists(j, i)) { delete E[j].remove(i); V[j].outDegree--; }
			
			Tv vBak = vertex(i); V.remvoe(i); //删除顶点i
			return vBak;
		}

		// 边的确认操作
		virtual bool exists(int i, intj) {
			return (0 <= i) && (i < n) && (0 <= j) && (j < n) && (E[i][j] != nullptr);
		}

		//边的基本操作
		virtual EStatus& status(int i, int j) { return E[i][j]->status; }
		virtual Te& edge(int i, int j) { return E[i][j]->data; }
		virtual int& weight(int i, int j) { return E[i][j]->weight; }

		//边的动态操作
		// 插入权重为w的e =(i,j)
		virtual void insert(Te const& edge, int w, int i, int j) {
			if (exist(i, j)) return; //确保边不存在
			E[i][j] = new Edge<T>(edge, w);
			e++;	V[i].outDegree++;	V[j].inDegree++;
		}

		// 删除, 并返回边内的数据
		virtual Te remove(int i, int j) {
			Te eBak = edge(i, j); delete E[i][j]; 
			E[i][j] = nullptr;
			e--;	V[i].outDegree--; V[j].inDegree--;
			return eBak;
		}
	};

	//全图，可能多个连通域
	template<typename Tv, typename Te>
	void Graph<Tv, Te>::bfs(int s) {
		reset(); 
		int clock = 0;
		int v = s;
		do // 逐一检查所有顶点
			if (UNDISCOVERED == status(v)) // 一旦遇到尚未发现的顶点
				BFS(v, clock); //从该顶点出发启动一次BFS
		while (s != (v = (++v % n))); //按序号检查，不漏不重
	}

	template <typename Tv, typename Te> //单个连通域
	void Graph<Tv, Te>::BFS(int v, int& clock) {
		queue<int> Q;
		status(v) = DISCOVERED; 
		Q.enqueue(v);
		while (!Q.empty()) {
			int v = Q.dequeue(); 
			dTime(v) = ++clock;
			for(int u=fristNbr(v); -1<u; u=nextNbr(v,u))//枚举v所有的邻居u
				if (UNDISCOVERED == status(u)) {
					status(u) = DISCOVERED; 
					Q.enqueue(u);
					status(v, u) = TREE;
					parent(u) = v; //引入树边拓展支撑树
				}
				else { //若u已经被发现，或者甚至已访问完毕
					status(v, u) = CROSS; //将(v,u)归类于跨边
				}
			status(v) = VISITED; //至此， 当前节点访问完毕
		}
	}

	//深度优先搜索DFS算法（全图）
	template <typename Tv, typename Te> 
	void Graph<Tv, Te>::dfs(int s) { //assert: 0 <= s < n
		reset(); int clock = 0; int v = s; //初始化
		do //逐一检查所有顶点
			if (UNDISCOVERED == status(v)) //一旦遇到尚未发现的顶点
				DFS(v, clock); //即从该顶点出发启动一次DFS
		while (s != (v = (++v % n))); //按序号检查，故不漏不重
	}

	//深度优先搜索DFS算法（单个连通域）
	template <typename Tv, typename Te> 
	void Graph<Tv, Te>::DFS(int v, int& clock) { //assert: 0 <= v < n
		dTime(v) = ++clock; status(v) = DISCOVERED; //发现当前顶点v
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //枚举v的所有邻居u
			switch (status(u)) { //并视其状态分别处理
			case UNDISCOVERED: //u尚未发现，意味着支撑树可在此拓展
				type(v, u) = TREE;   status(u) = DISCOVERED; parent(u) = v; DFS(u, clock); break;
			case DISCOVERED: //u已被发现但尚未访问完毕，应属被后代指向的祖先
				type(v, u) = BACKWARD; break;
			default: //u已访问完毕（VISITED，有向图），则视承袭关系分为前向边或跨边
				type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; break;
			}
		status(v) = VISITED; fTime(v) = ++clock; //至此，当前顶点v方告访问完毕
	}





	// 基于DFS的拓扑排序算法
	template <typename Tv, typename Te> //基于DFS的拓扑排序算法
	stack<Tv>* Graph<Tv, Te>::tSort(int s) { //assert: 0 <= s < n
		reset(); int clock = 0; int v = s;
		stack<Tv>* S = new stack<Tv>; //用栈记录排序顶点
		do {
			if (UNDISCOVERED == status(v))
				if (!TSort(v, clock, S)) { //clock并非必需
					/*DSA*/print(S);
					while (!S->empty()) //任一连通域（亦即整图）非DAG
						S->pop(); break; //则不必继续计算，故直接返回
				}
		} while (s != (v = (++v % n)));
		return S; //若输入为DAG，则S内各顶点自顶向底排序；否则（不存在拓扑排序），S空
	}

	template <typename Tv, typename Te> //基于DFS的拓扑排序算法（单趟）
	bool Graph<Tv, Te>::TSort(int v, int& clock, stack<Tv>* S) { //assert: 0 <= v < n
		dTime(v) = ++clock; status(v) = DISCOVERED; //发现顶点v
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //枚举v的所有邻居u
			switch (status(u)) { //并视u的状态分别处理
			case UNDISCOVERED:
				parent(u) = v; type(v, u) = TREE;
				if (!TSort(u, clock, S)) //从顶点u处出发深入搜索
					return false; //若u及其后代不能拓扑排序（则全图亦必如此），故返回并报告
				break;
			case DISCOVERED:
				type(v, u) = BACKWARD; //一旦发现后向边（非DAG），则
				return false; //不必深入，故返回并报告
			default: //VISITED (digraphs only)
				type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
				break;
			}
		status(v) = VISITED; S->push(vertex(v)); //顶点被标记为VISITED时，随即入栈
		return true; //v及其后代可以拓扑排序
	}


	// 基于DFS搜索框架的双连通域分解算法
	template <typename Tv, typename Te> void Graph<Tv, Te>::bcc(int s) { //基于DFS的BCC分解算法
		reset(); int clock = 0; int v = s; Stack<int> S; //栈S用以记录已访问的顶点
		do
			if (UNDISCOVERED == status(v)) { //一旦发现未发现的顶点（新连通分量）
				BCC(v, clock, S); //即从该顶点出发启动一次BCC
				S.pop(); //遍历返回后，弹出栈中最后一个顶点——当前连通域的起点
			}
		while (s != (v = (++v % n)));
	}

#define hca(x) (fTime(x)) //利用此处闲置的fTime[]充当hca[]
	template <typename Tv, typename Te> //顶点类型、边类型
	void Graph<Tv, Te>::BCC(int v, int& clock, stack<int>& S) { //assert: 0 <= v < n
		hca(v) = dTime(v) = ++clock; status(v) = DISCOVERED; S.push(v); //v被发现并入栈
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //枚举v的所有邻居u
			switch (status(u)) { //并视u的状态分别处理
			case UNDISCOVERED:
				parent(u) = v; type(v, u) = TREE; BCC(u, clock, S); //从顶点u处深入
				if (hca(u) < dTime(v)) //遍历返回后，若发现u（通过后向边）可指向v的真祖先
					hca(v) = __min(hca(v), hca(u)); //则v亦必如此
				else //否则，以v为关节点（u以下即是一个BCC，且其中顶点此时正集中于栈S的顶部）
					/*DSA*/ {
					/*DSA*/printf("BCC rooted at %c:", vertex(v));
					/*DSA*/Stack<int> temp; do { temp.push(S.pop()); print(vertex(temp.top())); } while (u != temp.top()); print(vertex(parent(u))); while (!temp.empty()) S.push(temp.pop());
					while (u != S.pop()); //弹出当前BCC中（除v外）的所有节点，可视需要做进一步处理
					/*DSA*/printf("\n");
					/*DSA*/
				}
				break;
			case DISCOVERED:
				type(v, u) = BACKWARD; //标记(v, u)，并按照“越小越高”的准则
				if (u != parent(v)) hca(v) = __min(hca(v), dTime(u)); //更新hca[v]
				break;
			default: //VISITED (digraphs only)
				type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
				break;
			}
		status(v) = VISITED; //对v的访问结束
	}
#undef hca




	// 优先级搜索
	template <typename Tv, typename Te> 
	template <typename PU>
	void Graph<Tv, Te>::pfs(int s, PU prioUpdater) { //assert: 0 <= s < n
		reset(); int v = s; //初始化
		do //逐一检查所有顶点
			if (UNDISCOVERED == status(v)) //一旦遇到尚未发现的顶点
				PFS(v, prioUpdater); //即从该顶点出发启动一次PFS
		while (s != (v = (++v % n))); //按序号检查，故不漏不重
	}

	template <typename Tv, typename Te> template <typename PU> //顶点类型、边类型、优先级更新器
	void Graph<Tv, Te>::PFS(int s, PU prioUpdater) { //优先级搜索（单个连通域）
		priority(s) = 0; status(s) = VISITED; parent(s) = -1; //初始化，起点s加至PFS树中
		while (1) { //将下一顶点和边加至PFS树中
			for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w)) //枚举s的所有邻居w
				prioUpdater(this, s, w); //更新顶点w的优先级及其父顶点
			for (int shortest = INT_MAX, w = 0; w < n; w++)
				if (UNDISCOVERED == status(w)) //从尚未加入遍历树的顶点中
					if (shortest > priority(w)) //选出下一个
					{
						shortest = priority(w); s = w;
					} //优先级最高的顶点s
			if (VISITED == status(s)) break; //直至所有顶点均已加入
			status(s) = VISITED; type(parent(s), s) = TREE; //将s及与其父的联边加入遍历树
		}
	} //通过定义具体的优先级更新策略prioUpdater，即可实现不同的算法功能



	template <typename Tv, typename Te> struct PrimPU { //针对Prim算法的顶点优先级更新器
		virtual void operator() (Graph<Tv, Te>* g, int uk, int v) {
			if (UNDISCOVERED == g->status(v)) //对于uk每一尚未被发现的邻接顶点v
				if (g->priority(v) > g->weight(uk, v)) { //按Prim策略做松弛
					g->priority(v) = g->weight(uk, v); //更新优先级（数）
					g->parent(v) = uk; //更新父节点
				}
		}
	};



	template <typename Tv, typename Te> //Prim算法：无向连通图，各边表示为方向互逆、权重相等的一对边
	void Graph<Tv, Te>::prim(int s) { //assert: 0 <= s < n
		reset(); priority(s) = 0;
		for (int i = 0; i < n; i++) { //共需引入n个顶点和n-1条边
			status(s) = VISITED;
			if (-1 != parent(s)) type(parent(s), s) = TREE; //引入当前的s
			for (int j = firstNbr(s); -1 < j; j = nextNbr(s, j)) //枚举s的所有邻居j
				if ((status(j) == UNDISCOVERED) && (priority(j) > weight(s, j))) //对邻接顶点j做松弛
				{
					priority(j) = weight(s, j); parent(j) = s;
				} //与Dijkstra算法唯一的不同之处
			for (int shortest = INT_MAX, j = 0; j < n; j++) //选出下一极短跨边
				if ((status(j) == UNDISCOVERED) && (shortest > priority(j)))
				{
					shortest = priority(j); s = j;
				}
		}
	}





	//针对Dijkstra算法的顶点优先级更新器
	template <typename Tv, typename Te> struct DijkPU { 
		virtual void operator() (Graph<Tv, Te>* g, int uk, int v) {
			if (UNDISCOVERED == g->status(v)) //对于uk每一尚未被发现的邻接顶点v，按Dijkstra策略
				if (g->priority(v) > g->priority(uk) + g->weight(uk, v)) { //做松弛
					g->priority(v) = g->priority(uk) + g->weight(uk, v); //更新优先级（数）
					g->parent(v) = uk; //并同时更新父节点
				}
		}
	};


	//最短路径Dijkstra算法：适用于一般的有向图
	template <typename Tv, typename Te> 
	void Graph<Tv, Te>::dijkstra(int s) { //assert: 0 <= s < n
		reset(); priority(s) = 0;
		for (int i = 0; i < n; i++) { //共需引入n个顶点和n-1条边
			status(s) = VISITED;
			if (-1 != parent(s)) type(parent(s), s) = TREE; //引入当前的s
			for (int j = firstNbr(s); -1 < j; j = nextNbr(s, j)) //枚举s的所有邻居j
				if ((status(j) == UNDISCOVERED) && (priority(j) > priority(s) + weight(s, j))) //对邻接顶点j做松弛
				{
					priority(j) = priority(s) + weight(s, j); parent(j) = s;
				} //与Prim算法唯一的不同之处
			for (int shortest = INT_MAX, j = 0; j < n; j++) //选出下一最近顶点
				if ((status(j) == UNDISCOVERED) && (shortest > priority(j)))
				{
					shortest = priority(j); s = j;
				}
		}
	} //对于无向连通图，假设每一条边表示为方向互逆、权重相等的一对边



};