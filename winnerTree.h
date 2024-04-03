#pragma once
#include<iostream>
using namespace std;
template<class T>
class winnerTree
{
public:
    winnerTree(T* thePlayer, int theNumberOfPlayers) {//构造函数
        this->tree = NULL;
        initialize(thePlayer, theNumberOfPlayers);
    }
    ~winnerTree() {//析构函数
        delete[] tree;
        tree = NULL;
    } 
    // 为数组初始化竞赛树
    void initialize(T* thePlayer, int theNumberOfPlayers);
    //返回最终赢者的节点索引
    int winner() const;
    //返回竞赛树某个节点的输者
    int winner(int i)const;
    //在参赛者thePLlayer变化后重赛，即重构
    void rePlay(int thePLayer);
protected:
    //对tree[p]进行比赛，如果还有父节点，继续向上比赛
    void play(int p, int leftChild, int rightChild);
private:
    int lowExt;            //最底层外部节点个数
    int offset;            //offset=2*s-1,s为最底层最左端内部节点，s=2^[log(n-1)];
    int* tree;             //最小赢者树
    int numberOfPlayers;   //竞赛选手数量
    T* player;             //保存竞赛选手
};

template<class T>
inline void winnerTree<T>::initialize(T* thePlayer, int theNumberOfPlayers)
{
    int n = theNumberOfPlayers;
    if (n < 2) {
        throw logic_error("the number of players must be at least 2");
    }
    //初始化类内数据成员
    this->numberOfPlayers = n;   //当前竞赛者数目
    this->player = thePlayer;    //竞赛者
    delete[]tree;                //删除旧竞赛树
    this->tree = new int[n];     //创建新竞赛树数组
    //计算s，s=2^[log(n-1)];
    int i, s;
    for (s = 1; 2 * s < n - 1; s += s);

    this->lowExt = 2 * (n - s);
    this->offset = 2 * s - 1;

    //为最低级别的外部节点进行匹配
    for (i = 2; i <= this->lowExt; i += 2) {
        play((this->offset + i) / 2, i - 1, i);
    }
    //处理剩余的外部节点
    if (n % 2 == 1) {//特殊情况n为奇数，发挥内部节点和外部节点
        play(n / 2, this->tree[n - 1], this->lowExt + 1);
        i = this->lowExt + 3;
    }
    else {
        i = this->lowExt + 2;
    }
    //i是最左边剩余的外部节点
    for (; i <= n; i += 2) {
        play((i - this->lowExt + n - 1) / 2, i - 1,i);
    }

}

template<class T>
inline int winnerTree<T>::winner() const
{
    return this->tree[1];
}

template<class T>
inline int winnerTree<T>::winner(int i) const
{
    return (i < numberOfPlayers) ? this->tree[i] : 0;
}

template<class T>
inline void winnerTree<T>::rePlay(int thePLayer)
{
    int n = numberOfPlayers;
    if (thePLayer <= 0 || thePLayer > n) {
        throw logic_error("player index is illegal");
    }
    int matchNode;
    int leftChild;
    int rightChild;
    //查找第一个匹配节点及其子节点
    if (thePLayer <= lowExt) {
        matchNode = (offset + thePLayer) / 2;
        leftChild = 2 * matchNode - offset;
        rightChild = leftChild + 1;
    }
    else
    {
        matchNode = (thePLayer - lowExt + n - 1) / 2;
        if (2 * matchNode == n - 1) {
            leftChild = tree[2 * matchNode];
            rightChild = thePLayer;
        }
        else {
            leftChild = 2 * matchNode - n + 1 + lowExt;
            rightChild = leftChild + 1;
        }
    }
    tree[matchNode] = (player[leftChild] <= player[rightChild]) ? leftChild : rightChild;
    if (matchNode == n - 1 && n % 2 == 1) {
        matchNode /= 2;
        tree[matchNode]= (this->player[tree[n - 1]] <= this->player[lowExt + 1]) ? tree[n - 1] : lowExt+1;
    }
    //玩剩下的比赛
    matchNode /= 2;//移至父节点
    while (matchNode >= 1) {
        tree[matchNode] =( (player[tree[2 * matchNode]] <= player[tree[2 * matchNode + 1]]) ? tree[2 * matchNode] : tree[2 * matchNode + 1]);
        matchNode /= 2;//移至父节点
    }
}

template<class T>
inline void winnerTree<T>::play(int p, int leftChild, int rightChild)
{
    //最小赢者树，返回最小赢者
    this->tree[p] = (this->player[leftChild] <= this->player[rightChild]) ? leftChild : rightChild;
    while (p % 2 == 1 && p > 1) {//p为右子节点且还有父节点，继续向上比赛
        //对父节点进行比赛
        this->tree[p / 2] = (this->player[tree[p - 1]] <= this->player[tree[p]]) ? tree[p - 1] : tree[p];
        //向上移至父节点
        p /= 2;
    }
}
