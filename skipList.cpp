#include <iostream>
#include <math.h>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include"skiplist.h"
using namespace std;

const int X = 1, Y = 100000;  //随机生成区间[x,y]之间的数
int M = 1000, N = 100;        //M为初始化数量，N为操作数量
int op,temp,testNum;          //op为操作选择数，temp为对应操作数,testNum为测试数据量
int a[1000000];
bool flag = false;
pair<int, int>thePair;

bool Search(int a[], int length, int key) {//普通一维数组线性查找                        
    for(int i=0;i<length;i++) {
        if (a[i] == key)return true;
    }
    return false;
}
bool BinSearch(int a[], int length, int key) {//普通一维数组折半查找                        
    int left = 0, right = length - 1;
    int mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (a[mid] == key) {
            return true;
        }
        else if (a[mid] < key) {
            left = mid + 1;
        }
        else if (a[mid] > key) {
            right = mid - 1;
        }
    }
    return false;
}
void func1() {//使用跳表各操作功能
        
        cout << "请输入初始化数据量M: ";
        cin >> M;
        cout << "请输入跳表操作次数N: ";
        cin >> N;
        skipList<int, int>list;
        for (int i = 0; i < M; i++) {//初始化操作
            temp = (rand() % (Y - X + 1)) + X;//生成随机数
            list.initialize(temp);
        }
        for (int i = 0; i < N; i++) {//N次操作
            op = (rand() % (5 - 1 + 1)) + 1;//生成随机操作
            cout << op << " ";
            switch (op) {
            case 1://操作1，插入
                temp = (rand() % (Y - X + 1)) + X;
                thePair = (make_pair(temp, temp));
                list.insert(thePair);
                cout << temp << endl;
                break;
            case 2://操作2，查找
                temp = (rand() % (Y - X + 1)) + X;
                cout << temp << " ";
                flag = list.find(temp);
                if (flag) {
                    cout << "Is found" << endl;
                }
                else {
                    cout << "Not found" << endl;
                }
                break;
            case 3://操作3，删除
                temp = (rand() % (Y - X + 1)) + X;
                list.erase(temp);
                cout << temp << endl;
                break;
            case 4:
                thePair = list.eraseMin();             
                cout << "(" << thePair.first << "," << thePair.second << ")" << endl;
                break;
            case 5:
                thePair = list.eraseMax();                  
                cout << "(" << thePair.first << "," << thePair.second << ")" << endl;

                break;           
            }
        }
        //list.output();//输出跳表所有节点
}
void func2() {//测试跳表各功能的时间与效率

    cout << "请输入要测试的数据量：";
    cin >> testNum;

    skipList<int, int>list;
    clock_t start1 = clock();//插入操作，初态
    for (int i = 0; i < testNum; i++) {//插入操作2，同时也是初始化操作
        temp = (rand() % (Y - X + 1)) + X;//生成随机数
        list.initialize(temp);
    }
    clock_t end1 = clock();//插入操作，末态
    double time1 = (double)(1000 * (end1 - start1)) / CLOCKS_PER_SEC;//记录插入操作时间

   //为数组插入数据
   list.inputArry(a);//将跳表中的元素放入一维数组中,保证数据一致性
   //for (int i = 0; i < list.size(); i++)cout << a[i] << " ";
   clock_t start6 = clock();//数组顺序查找操作，初态
   for (int i = 0; i < testNum; i++) {
       temp = (rand() % (Y - X + 1)) + X;//生成随机数;
       Search(a, testNum, temp);
   }
   clock_t end6 = clock();//数组顺序查找操作，末态
   double time6 = (double)(1000 * (end6 - start6)) / CLOCKS_PER_SEC;//记录数组顺序查找操作时间

   clock_t start7 = clock();//数组折半查找操作，初态
   for (int i = 0; i < testNum; i++) {
       temp = (rand() % (Y - X + 1)) + X;//生成随机数;
       BinSearch(a, testNum, temp);
   }
   clock_t end7 = clock();//数组折半查找操作，末态
   double time7 = (double)(1000 * (end7 - start7))/ CLOCKS_PER_SEC;//记录数组折半查找操作时间


    clock_t start2 = clock();//查找操作，初态
    for (int i = 0; i < testNum; i++) {//查找操作1
        temp = (rand() % (Y - X + 1)) + X;//生成随机数
        list.find(temp);
    }
    clock_t end2 = clock();//查找操作，末态
    double time2 = (double)(1000 * (end2 - start2)) / CLOCKS_PER_SEC;//记录查找操作时间

    clock_t start3 = clock();//删除操作，初态
    for (int i = 0; i < testNum; i++) {//删除操作3
        temp = (rand() % (Y - X + 1)) + X;//生成随机数
        list.erase(temp);
    }
    clock_t end3 = clock();//删除操作，末态
    double time3 = (double)(1000 * (end3 - start3)) / CLOCKS_PER_SEC;//记录删除操作时间

    for (int i = 0; i < testNum; i++) {//补充数据
        temp = (rand() % (Y - X + 1)) + X;//生成随机数;
        list.initialize(temp);
    }
    clock_t start4 = clock();//删除最小值操作，初态
    for (int i = 0; i < testNum; i++) {//删除操作4
        list.eraseMin();
    }
    clock_t end4 = clock();//删除最小值操作，末态
    double time4 = (double)(1000 * (end4 - start4)) / CLOCKS_PER_SEC;//记录删除最小值操作时间

    for (int i = 0; i < testNum; i++) {//补充数据
        temp = (rand() % (Y - X + 1)) + X;//生成随机数;
        list.initialize(temp);
    }
    clock_t start5 = clock();//删除最大值操作，初态
    for (int i = 0; i < testNum; i++) {//删除操作5
        list.eraseMax();
    }
    clock_t end5 = clock();//删除最大值操作，末态
    double time5 = (double)(1000 * (end5 - start5)) / CLOCKS_PER_SEC;//记录删除最大值操作时间

    cout << testNum << "次插入操作的平均时间为：" << time1 << "ms" << endl;
    cout << testNum << "次查找操作的平均时间为：" << time2 << "ms" << endl;
    cout << testNum << "次删除某节点操作的平均时间为：" << time3 << "ms" << endl;
    cout << testNum << "次删除最小值操作的平均时间为：" << time4 << "ms" << endl;
    cout << testNum << "次删除最大值操作的平均时间为：" << time5 << "ms" << endl;
    cout << testNum << "次一维数组顺序查找的平均时间为：" << time6 << "ms" << endl;
    cout << testNum << "次一维数组折半查找的平均时间为：" << time7 << "ms" << endl << endl;

}
int main() { 
  
    srand((unsigned)time(NULL)*10);
    cout << "测试操作请按1：" << endl;
    cout << "测试用时请按2：" << endl;
    int n;
    cin >> n;
    if(n==1)func1();
    if(n==2)func2();
    return 0;
}
