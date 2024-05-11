#pragma once
#include<bits/stdc++.h>
#include<windows.h>
#include<time.h>
#include"graph.h"
using namespace std;
void solution1() {
	for (int i = 1; i <= 100; i++) {
		string fin1 = "D:/CodeFile/VS/ex-数据结构与算法设计基础实验/ex-网络放大器设置问题/input/input";
		fin1 += to_string(i) + ".in";
		string fout1 = "D:/CodeFile/VS/ex-数据结构与算法设计基础实验/ex-网络放大器设置问题/output/";
		fout1 += to_string(i) + ".out";
		fin.open(fin1);
		fout.open(fout1);
		if (!fin.is_open() || !fout.is_open()) {
			cout << "文件无法打开！\n";
			//return;
		}
		Digraph g;
		g.topoSort();
		g.backTrack(1, 0);
		fout << g.bestAns << endl;
		fin.close();
		fout.close();

		cout << "正在生成可视化图片 " << i << endl;
		g.visualization1(i);
		cout << "生成完毕.\n";
	}
	/*cout << "正在进行对拍...\n";
	check();*/
}
void solution1Chart() {
	clock_t start = clock();
	for (int i = 1; i <= 100; i++) {
		string fin1 = "D:/CodeFile/VS/ex-数据结构与算法设计基础实验/ex-网络放大器设置问题/input/input";
		fin1 += to_string(i) + ".in";
		string fout1 = "D:/CodeFile/VS/ex-数据结构与算法设计基础实验/ex-网络放大器设置问题/output/";
		fout1 += to_string(i) + ".out";
		fin.open(fin1);
		fout.open(fout1);
		if (!fin.is_open() || !fout.is_open()) {
			cout << "文件无法打开！\n";
			//return;
		}
		LARGE_INTEGER cpuFreq;
		LARGE_INTEGER startTime;
		LARGE_INTEGER endTime;

		QueryPerformanceFrequency(&cpuFreq);
		QueryPerformanceCounter(&startTime);

		Digraph g;
		g.topoSort();
		g.backTrack(1, 0);
		fout << g.bestAns << endl;
		QueryPerformanceCounter(&endTime);
		
		fin.close();
		fout.close();
		ofstream out("cost1.txt",ios::app);
		double time = (((static_cast<double>(endTime.QuadPart) - startTime.QuadPart) * 1000000) / cpuFreq.QuadPart);
		//out << i << "  " <<  time << "\n";//使用Python生成图表时使用这个输入，单位us.
		out << time << "us\n";				//使用Excel表或者网上在线软件生成图表时使用这个输入
		out.close();

	}
	//记录测试100个测试集的时间
	clock_t end = clock();
	double time= (double)(1000 * (end - start)) / CLOCKS_PER_SEC;
	ofstream out("cost1.txt", ios::app);
	out << "\n" << time << "ms\n\n";
	out.close();
}
