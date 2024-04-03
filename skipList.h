#pragma once
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
using namespace std;
//结构skipNode
template <class K, class E>
struct skipNode {
    typedef pair<const K, E>pairType;

    pairType element;
    skipNode<K, E>** next;//指针数组

    skipNode(const pairType& thePair, int size) :element(thePair) {
        next = new skipNode<K, E>*[size];
    }
};
//类skipList
template <class K, class E>
class skipList
{

public:
    skipList(K largeKey = 100000, int maxPairs = 10000, float prob = 0.5);
    ~skipList();

    bool empty() const { return dSize == 0; }
    int size() const { return dSize; }
    pair<const K, E>* find(const K&) const;
    void initialize(int n);
    void erase(const K&);
    pair<const K, E> eraseMin();
    pair<const K, E> eraseMax();
    void insert(const pair<const K, E>&);
    void output() const;
    void inputArry(int a[]);

protected:
    float cutOff;          // 用来确定最大层数
    int level() const;     // 级的分配方法，生成一个表示链表级的随机数，不大于maxLevel
    int levels;            // 当前最大的非空链表
    int dSize;             // 字典的数对个数
    int maxLevel;          // 允许的最大链表层数
    K tailKey;             // 最大关键字
    skipNode<K, E>* search(const K&) const; // 搜索，保存看到的最后一个节点到数组last中
    skipNode<K, E>* headerNode;  // 头节点指针
    skipNode<K, E>* tailNode;    // 尾节点指针
    skipNode<K, E>** last;       // last[i]表示i层的最后节点

};

template<class K, class E>
skipList<K, E>::skipList(K largeKey, int maxPairs, float prob)
{//构造函数
    cutOff = prob * RAND_MAX;
    maxLevel = (int)ceil(logf((float)maxPairs) / logf(1 / prob)) - 1;
    levels = 0;
    dSize = 0;
    tailKey = largeKey;

    //生成头节点、尾节点和数组last
    pair<K, E>tailPair;
    tailPair.first = tailKey;
    headerNode = new skipNode<K, E>(tailPair, maxLevel + 1);
    tailNode = new skipNode<K, E>(tailPair, 0);
    last = new skipNode<K, E> *[maxLevel + 1];

    //链表为空时，任意级链表中的头节点都指向尾节点
    for (int i = 0; i <= maxLevel; i++)
        headerNode->next[i] = tailNode;
}

template<class K, class E>
skipList<K, E>::~skipList()
{// 析构函数
    skipNode<K, E>* nextNode;

    // 删除0级链表中所有节点
    while (headerNode != tailNode)
    {
        nextNode = headerNode->next[0];
        delete headerNode;
        headerNode = nextNode;
    }
    delete tailNode;

    delete[] last;

}

template<class K, class E>
pair<const K, E>* skipList<K, E>::find(const K& theKey) const //时间复杂度平均O(logn)，最坏O(n)
{// 返回匹配的数对的指针
 // 如果没有匹配的数对，返回NULL;
    if (theKey >= tailKey)
        return NULL;  // 没有可能的匹配的数对

    // 位置 beforeNode是关键字为theKey得节点之前的最右边的位置
    skipNode<K, E>* beforeNode = headerNode;
    for (int i = levels; i >= 0; i--)          // 从上级链表到下级链表
        // 跟踪i级链表指针
        while (beforeNode->next[i]->element.first < theKey)
            beforeNode = beforeNode->next[i];

    // 检查下一个节点的关键字是否是theKey
    if (beforeNode->next[0]->element.first == theKey)
        return &beforeNode->next[0]->element;

    return NULL;  //无匹配的数对
}
template<class K, class E>
void skipList<K, E>::initialize(int n)
{//初始化，key值与value值相同
    pair<K, E> thePair(make_pair(n,n));
    insert(thePair);
}
template<class K, class E>
void skipList<K, E>::erase(const K& theKey)//时间复杂度平均O(logn)，最坏O(n)
{//删除关键字等于theKey的数对
    if (theKey >= tailKey) // 关键字太大
        return;

    // 查看是否有匹配的数对
    skipNode<K, E>* theNode = search(theKey);
    if (theNode->element.first != theKey) // 不存在
        return;

    // 从跳表中删除节点
    for (int i = 0; i <= levels &&
        last[i]->next[i] == theNode; i++)
        last[i]->next[i] = theNode->next[i];

    // 更新链表级
    while (levels > 0 && headerNode->next[levels] == tailNode)
        levels--;

    delete theNode;
    dSize--;
    
}
template<class K, class E>
pair<const K, E> skipList<K, E>::eraseMin()//O(1)复杂度
{//删除关键字最小的数对

    //链表为空，退出
    if (headerNode->next[0] == tailNode)
    {
        pair<const K, E> thePair(make_pair(-1, -1));
        return thePair;
    }
        

    //删除节点
    skipNode<K, E>* theNode = headerNode->next[0];
    pair<const K, E> thePair = theNode->element;
    //cout << "(" << theNode->element.first << " " << theNode->element.second << ")" < endl;
    for (int i = 0; i <= levels &&
        headerNode->next[i] == theNode; i++)
        headerNode->next[i] = theNode->next[i];

    // 更新链表级
    while (levels > 0 && headerNode->next[levels] == tailNode)
        levels--;

    delete theNode;
    dSize--;
    return thePair;
}
template<class K, class E>
pair<const K, E> skipList<K, E>::eraseMax() //时间复杂度平均O(logn)，最坏O(n)
{//删除关键字最大的数对
    //链表为空，退出
    if (headerNode->next[0] == tailNode) {
        pair<const K, E> thePair(make_pair(-1, -1));
        return thePair;
    }

    skipNode<K, E>* beforeNode = headerNode;
    for (int i = levels; i >= 0; i--)
    {
        if (beforeNode->next[i] == tailNode)
            continue;
        while (beforeNode->next[i]->next[i] != tailNode)
            beforeNode = beforeNode->next[i];
        last[i] = beforeNode;  // 最后一级链表i的节点
    }
    //删除节点
    skipNode<K, E>* theNode = beforeNode->next[0];
    pair<const K, E> thePair = theNode->element;
    //cout << "(" << theNode->element.first << " " << theNode->element.second << ")" < endl;
    for (int i = 0; i <= levels &&
        last[i]->next[i] == theNode; i++)
        last[i]->next[i] = theNode->next[i];

    // 更新链表级
    while (levels > 0 && headerNode->next[levels] == tailNode)
        levels--;

    delete theNode;
    dSize--;
    return thePair;
}
template<class K, class E>
int skipList<K, E>::level() const
{// 返回一个表示链表级的随机数，且不大于maxLevel
    int lev = 0;
    while (rand() <= cutOff)
        lev++;
    return (lev <= maxLevel) ? lev : maxLevel;
}
template<class K, class E>
skipNode<K, E>* skipList<K, E>::search(const K& theKey) const
{// 搜索关键字theKey，把每一级链表中要查看的最后一个节点存储在数组last中
 //返回包含关键字theKey的节点
   // 位置 beforeNode是关键字为theKey得节点之前最右边的位置
    skipNode<K, E>* beforeNode = headerNode;
    for (int i = levels; i >= 0; i--)
    {
        while (beforeNode->next[i]->element.first < theKey)
            beforeNode = beforeNode->next[i];
        last[i] = beforeNode;  // 最后一级链表i的节点
    }
    return beforeNode->next[0];
}

template<class K, class E>
void skipList<K, E>::insert(const pair<const K, E>& thePair) //时间复杂度平均O(logn)，最坏O(n)
{// 把数对thePair插入字典，覆盖已存在的相同数对
    if (thePair.first >= tailKey) // 关键字太大
    {
        ostringstream s;
        s << "Key = " << thePair.first << " Must be < " << tailKey;
        throw logic_error(s.str());
    }

    // 查看关键字为theKey的数对是否已经存在
    skipNode<K, E>* theNode = search(thePair.first);
    if (theNode->element.first == thePair.first)
    {// 若存在则更新该数对的值
        theNode->element.second = thePair.second;
        return;
    }

    // 若不存在，则确定新节点所在的级链表
    int theLevel = level(); // 新节点的级
    // 使级 theLevel 为 <= levels + 1
    if (theLevel > levels)
    {
        theLevel = ++levels;
        last[theLevel] = headerNode;
    }

    // 在节点theNode后插入新节点
    skipNode<K, E>* newNode = new skipNode<K, E>(thePair, theLevel + 1);
    for (int i = 0; i <= theLevel; i++)
    {// 插入i级链表
        newNode->next[i] = last[i]->next[i];
        last[i]->next[i] = newNode;
    }

    dSize++;
    return;
}
template<class K, class E>
void skipList<K, E>::output() const
{//输出0级链表节点
    for (skipNode<K, E>* currentNode = headerNode->next[0];
        currentNode != tailNode;
        currentNode = currentNode->next[0])
        cout<<"(" << currentNode->element.first << ","<< currentNode->element.first << ")->";
}

template<class K, class E>
inline void skipList<K, E>::inputArry(int a[])
{//用于将跳表中的元素关键字置入一维数组中
    int i = 0;
    for (skipNode<K, E>* currentNode = headerNode->next[0];
        currentNode != tailNode;
        currentNode = currentNode->next[0])
        a[i++] = currentNode->element.first;
}

