#include<bits/stdc++.h>
#include"graph.h"
#include"solution1.h"
#include"solution2.h"
using namespace std;
ifstream fin;
ofstream fout;
//性能分析
void test() {
	cout << "正在生成性能对比数据...\n";
	solution1Chart();
	solution2Chart();
	cout << "数据结果已生成，请绘制图表进行分析.\n";
	//system("testTime.py");//我这里没跑起来，你自己看
}
//test1:测试100个随机样例
void test1() {
	cout << "正在测试方法1:dfs回溯...\n";
	solution1();
	cout<< "正在测试方法2:分支定界...\n";
	solution2();
}
//test2:测试指定数据集合
void test2() {
	cout << "请输入文件路径名称：\n";
	string path;
	cin >> path;
	fin.open(path);
	if (!fin.is_open()) {
		cout << "文件无法打开，请重新输入！\n";
		return;
	}
	Digraph g;
	g.topoSort();
	//g.backTrack(1,0);
	g.branchBound();
	cout << g.bestAns << endl;
	g.outputBst();
}
int main() {
	while (1) {
		cout << "------放大器网络------\n";
		cout << "0:进行性能对比分析\n";
		cout << "1:测试100个测试样例\n";
		cout << "2:测试指定数据集合\n";
		string op;
		cin >> op;
		if (op == "0") {
			test();
		}
		else if (op == "1") {
			test1();
		}
		else if (op == "2") {
			test2();
		}
		else {
			cout << "输入错误，请重新输入！\n";
		}
	}
	return 0;
}