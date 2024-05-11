#pragma once
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
extern ifstream fin;
extern ofstream fout;
struct Node {
	Node* father;
	int level;					//子集树层级
	int press;					//压力值
	bool booster;				//是否放置放大器				
	int maxCost;				//最大开销
	int bstNum;				    //优先级，从根到节点x路径上的bst个数
	Node() {
		father = NULL;
		press = 0;
		level = 0;
		bstNum = 0;
		maxCost = 0;
		booster = false;
	}
	Node(int pre, int lev, Node* fa = NULL, int bst = 0) {
		father = fa;
		press = pre;
		level = lev;
		bstNum = bst;
		maxCost = 0;
		booster = false;
	}
	bool operator<(const Node& b)const {
		return bstNum > b.bstNum;
	}
};

struct outEdge {
	int v;	//可达的点
	int w;	//权值长度
	outEdge(int to, int w) :v(to), w(w) {};
};
struct inEdge {
	int u;
	int w;
	inEdge(int u, int w) :u(u), w(w) {};
};
struct Vertex {
	vector<outEdge>outEdges;	//记录出边
	vector<inEdge>inEdges;		//记录入边，用于更新press
	int press;					//压力
	bool booster;				//放大器				
	int maxCost;				//最大开销
	int topoNum;				//拓扑次序
	Vertex() {
		press = 0;
		booster = false;
		outEdges.clear();
		inEdges.clear();
		maxCost = 0;
		topoNum = 0;
	}
};

class Digraph {
public:
	int n, m;								//顶点数，边数
	int bestAns;							//最优解数目
	Digraph();								//构造函数
	~Digraph() {};							//析构函数
	void visualization1(int i);				//可视化输出1
	void visualization2(int i);				//可视化输出2
	void topoSort();						//拓扑排序确定先后关系
	void updatePress();						//更新压力值
	void outputBst();						//输出放大器
	void backTrack(int level, int cnt);		//dfs回溯解决
	void branchBound();						//分支定界解决
private:
	int Pmin, Pmax;
	vector<Vertex>vertexs;
	vector<int>inDeg;		//入度
	vector<int>topology;    //拓扑序列
	vector<bool>isBst;		//是否放置放大器

	vector<Vertex>ver;
	priority_queue<Node*, vector<Node*>>q;//最小堆
};
Digraph::Digraph()
{
	fin >> n >> m >> Pmax;
	Pmin = 0;				//最小压力设置为0
	bestAns = INT_MAX;
	vertexs.clear();
	inDeg.resize(n + 1, 0);
	topology.resize(n + 1, 0);
	isBst.resize(n + 1, 0);

	for (int i = 0; i <= n; i++) {
		vertexs.push_back(Vertex());
	}
	for (int i = 0; i < m; i++) {
		int u, v, w;
		fin >> u >> v >> w;
		//记录出边
		vertexs[u].outEdges.emplace_back(v, w);
		//更新入度
		inDeg[v]++;
		//更新最大花销
		vertexs[u].maxCost = max(vertexs[u].maxCost, w);
		//记录入边
		vertexs[v].inEdges.emplace_back(u, w);
	}
	//更新压力
	vertexs[1].press = Pmax;
	isBst[1] = true;

	/*ver = vertexs;
	bfs2(1);*/
}

void Digraph::visualization1(int a)
{
	updatePress();
	string fileName =  to_string(a) + ".dot";
	ofstream out(fileName);
	out << "digraph g {\n";
	for (int i = 1; i < vertexs.size(); i++) {
		out << i;
		if (isBst[i]) {
			out << "[label=\"vertex" << i << ";pre = " << vertexs[i].press << "\",style=filled,fillcolor=red];\n";
		}
		else {
			out << "[label=\"vertex" << i << ";pre = " << vertexs[i].press << "\",style=filled,fillcolor=white];\n";
		}
	}
	for (int i = 1; i <= n; i++) {
		for (auto& ed : vertexs[i].outEdges) {
			out << i << "->" << ed.v << "[label=\"cost=" << ed.w << "\"];\n";
		}
	}
	out << 0;
	out << "[label=\"Pmax" << Pmax << ";Pmin = " << Pmin << "\",style=filled,fillcolor=yellow];\n";
	out << "}\n";
	out.close();
	string order = "dot -Tjpg " + to_string(a) + ".dot -o solve" + to_string(a) + ".jpg";
	system(order.c_str());
}

void Digraph::visualization2(int a)
{
	updatePress();
	string fileName = to_string(a) + "-2.dot";
	ofstream out(fileName);
	out << "digraph g {\n";
	for (int i = 1; i < vertexs.size(); i++) {
		out << i;
		if (isBst[i]) {
			out << "[label=\"vertex" << i << ";pre = " << vertexs[i].press << "\",style=filled,fillcolor=red];\n";
		}
		else {
			out << "[label=\"vertex" << i << ";pre = " << vertexs[i].press << "\",style=filled,fillcolor=white];\n";
		}
	}
	for (int i = 1; i <= n; i++) {
		for (auto& ed : vertexs[i].outEdges) {
			out << i << "->" << ed.v << "[label=\"cost=" << ed.w << "\"];\n";
		}
	}
	out << "}\n";
	out.close();
	string order = "dot -Tjpg " + to_string(a) + "-2.dot -o solve" + to_string(a) + "-2.jpg";
	system(order.c_str());
}

void Digraph::topoSort()
{//使用bfs遍历确定拓扑序列
 //每次确定一个顶点的拓扑序列后，更新受影响的点的入度
	int cnt = 1;
	queue<int>q;
	for (int i = 1; i <= n; i++) {
		if (inDeg[i] == 0)q.push(i);
	}
	while (!q.empty()) {
		int u = q.front();
		q.pop();

		vertexs[u].topoNum = cnt;

		topology[cnt++] = u;		//排序

		for (auto& ed : vertexs[u].outEdges) {
			if (--inDeg[ed.v] == 0)q.push(ed.v);
		}
	}
	if (cnt < n) {
		cout << "图中有环，不合要求！\n";
		return;
	}
}

void Digraph::updatePress()
{
	for (int i = 2; i <= n; i++) {
		int u = topology[i];
		if (isBst[u]) {
			vertexs[u].press = Pmax;
		}
		else {
			vertexs[u].press = 0;
			for (int j = 1; j < i; j++) {
				for (auto& ed : vertexs[topology[j]].outEdges) {
					if (ed.v == u) {
						vertexs[u].press = max(vertexs[u].press, vertexs[topology[j]].press - ed.w);
					}
				}
			}
		}
	}
}

void Digraph::outputBst()
{
	for (int i = 1; i <= n; i++) {
		if (isBst[i]) {
			cout << "vertex " << i << endl;
		}
	}
}

void Digraph::backTrack(int i, int cnt)
{//进行dfs回溯，最坏时间复杂度O(2^n)
    //搜索第i层 
	int u = topology[i];//当前点i也代表了拓扑序

	if (i >= n) {//到最后一个节点了，更新信息，递出
		//更新最优解
		bestAns = min(cnt, bestAns);
		//更新放置情况
		for (int j = 1; j <= n; j++) {
			isBst[j] = false;
			isBst[j] = vertexs[j].booster;
		}
		return;
	}

	if (i == 1) {//第一层 源点不需要放置
		backTrack(2, 0);
	}
	else {//判断其他层  若到点v的压力值小于Pmin, 则在点u放置
		int x;
		int flag = 0;
		int tempPress = -1;//当前压力值，用来剪枝回溯

		for (int k = 1; k < i; k++) {//第一层到当前层 
			for (auto& ed : vertexs[topology[k]].outEdges) {
				if (ed.v == u)//更新压力值
					tempPress = max(tempPress, vertexs[topology[k]].press - ed.w);
			}
		}

		for (x = 1; x <= 2; x++)
		{//进行两种决策
			//剪枝1，当前压力值为Pmax或已放置放大器
			if (tempPress >= Pmax || vertexs[u].booster == true) {
				backTrack(i + 1, cnt);
				break;
			}
			//剪枝2，当前压力值小于Pmin，必须放置
			if (tempPress < Pmin)
			{
				cnt++;
				vertexs[u].booster = true;
				//剪枝3，其他层已经找到更优解了
				if (cnt >= bestAns) {
					return;
				}
				backTrack(i + 1, cnt);
			}
			if (x == 1) {//1状态时先对所有出边进行判断
				for (auto& v : vertexs[u].outEdges) {
					int nextPress = tempPress - v.w;
					if (nextPress < Pmin) {//下一个节点压力值小于Pmin，需要放置
						flag = 1;
						break;
					}
				}
			}
			//放置
			if (x == 2 || flag == 1) {
				cnt++;
				vertexs[u].press = Pmax;
				vertexs[u].booster = true;
				//剪枝3，其他层已经找到更优解了
				if (cnt >= bestAns)
					return;
				else {
					backTrack(i + 1, cnt);
				}
			}
			//不放置
			else if (x == 1 && flag != 1) {
				vertexs[u].press = tempPress;
				vertexs[u].booster = false;
				backTrack(i + 1, cnt);
			}
		}
	}
}

void Digraph::branchBound()
{//最小耗费，分支定界
	Node* eNode = new Node(Pmax, 2);//设置活节点，
	int level = 2;					//从第2层开始
	while (level <= n - 1) {		//扩展活节点
		int u = topology[level];//当前层扩展的活节点
		int tempPress = -1;		//扩展节点压力值
		int flag = 0;

		for (int k = 1; k <= level - 1; k++) {//计算扩展节点压力值
			for (auto& e : vertexs[topology[k]].outEdges) {
				if (e.v == u) {
					Node* p = eNode;
					for (int j = level - 1; j > k; j--) {
						p = p->father;
					}
					tempPress = max(tempPress, p->press - e.w);
				}
			}
		}
		//进行限界判断，当前压力值最大损耗后小于Pmin,需要放置
		if (tempPress - vertexs[u].maxCost < Pmin) {
			flag = 1;
		}
		if (flag == 0) {//两种情况 放置或不放置
			//不放置
			Node* t = new Node(tempPress, level + 1, eNode, eNode->bstNum);
			q.push(t);

			//放置
			Node* s = new Node(Pmax, level + 1, eNode, eNode->bstNum + 1);
			s->booster = true;
			q.push(s);
		}
		else {//必须放置
			Node* s = new Node(Pmax, level + 1, eNode, eNode->bstNum + 1);
			s->booster = true;
			q.push(s);
		}
		eNode = q.top();//更新活节点
		q.pop();
		level = eNode->level;//获取层数
	}

	bestAns = eNode->bstNum;//更新最优解

	Node* p = eNode;
	while (p) {//更新isBst的状态
		isBst[topology[p->level - 1]] = p->booster;
		p = p->father;
	}

}
//对拍函数，用于对照结果是否正确
void check() {
	int flag = 0;
	stringstream s;
	for (int i = 1; i <= 100; i++) {
		cout << "正在测试数据集：" << i << endl;
		string opStd = "D:/CodeFile/VS/ex-数据结构与算法设计基础实验/ex-网络放大器设置问题/outputSTD/";
		string myin = "D:/CodeFile/VS/ex-数据结构与算法设计基础实验/ex-网络放大器设置问题/output/";
		opStd += to_string(i) + ".out";
		myin += to_string(i) + ".out";
		ifstream finstd(opStd);
		if (!finstd.is_open()) {
			cout << "文件无法打开\n";
		}
		int ans;
		finstd >> ans;
		finstd.close();

		ifstream finmy(myin);
		if (!finmy.is_open()) {
			cout << "文件无法打开\n";
		}
		int me;
		finmy >> me;
		finmy.close();
		if (me != ans) {
			cout << "Wrong Answer\n";
			s << i << "WA!\n";
		}
		else {
			cout << "Accept\n";
		}
	}
	if (!flag) {
		cout << "All Accept!\n";
	}
	else {
		cout << "Wrong Answer\n";
	}
	cout << s.str();
}