//#include <iostream>
//#include <cstdlib>
//#include <ctime>
//#include <fstream>
//using namespace std;
//const int x = 1, y = 1000;  //随机生成区间[x,y]之间的数
//const int m = 100, n = 30;//m为初始化数量，n为操作数量,分十次生成随机数和随机操作
//int op,temp;
//int main()
//{
//	srand((unsigned)time(NULL)*10);
//
//	ofstream fout1, fout2;
//	fout1.open("d:\\codefile\\vs\\ex3数据结构算法设计实验\\input1.txt", ios::app | ios::out);
//	fout2.open("d:\\codefile\\vs\\ex3数据结构算法设计实验\\input.txt", ios::app | ios::out);
//	if (!fout1.is_open() || !fout2.is_open()) { cout << "error" << endl; }
//
//	for (int i = 0; i < m; i++)//生成m个随机数用于初始化跳表
//	{
//		fout1 << ((rand() % (y - x + 1)) + x) << '\n';//取得[x,y]区间内随机数
//	}
//	for (int i = 0; i < n; i++) 
//	{
//		if (i < (n / 3)) {//生成插入操作2
//			op = 2;
//			temp = (rand() % (y - x + 1)) + x;//生成随机数
//			fout2 << op << " " << temp << '\n';
//		}
//		else if (i >= (n / 3) && i < (2 * n / 3)) {//生成删除操作3,4,5
//			op = (rand() % (5 - 3 + 1)) + 3;
//			if (op == 3) {//生成删除操作3
//				temp = (rand() % (y - x + 1)) + x;//生成随机数
//				fout2 << op << " " << temp << '\n';
//			}
//			else if (op == 4 || op == 5) fout2 << op << '\n';//生成删除最大值或最小值操作4,5
//		}
//		else {//生成查找操作1
//			op = 1;
//			temp = (rand() % (y - x + 1)) + x;//生成随机数
//			fout2 << op << " " << temp << '\n';
//		}
//	}
//	cout << "成功生成随机数和随机操作" << endl;
//	fout1.close();
//	fout2.close();
//	return 0;
//}