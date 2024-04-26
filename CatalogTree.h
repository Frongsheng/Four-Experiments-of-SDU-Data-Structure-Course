#pragma once
#include<bits/stdc++.h>
using namespace std;
struct CatalogNode {
	string name;					//当前目录名称
	bool isFile;					//标识位，标记叶节点
	CatalogNode* father;			//记录父节点
	vector<CatalogNode*>next;		//当前目录下的子目录
	CatalogNode(const string& Name, CatalogNode* father = nullptr, bool isFile = false) :name(Name), father(father), isFile(isFile) {}
	bool operator<(const CatalogNode& b)const {
		return name < b.name;
	}
};
class CatalogTree {
public:
	//构造函数
	CatalogTree();
	//列出当前目录下的所有目录文件项
	void Dir();
	//打出当前目录的绝对路径
	void Cd();
	//当前目录变为当前目录的父目录
	void CdUp();
	//当前目录变为str表示路径的目录
	void CdTo(const string& str);
	//在当前目录下创建一个子目录str
	void Mkdir(const string& str);
	//在当前目录下创建一个文件str
	void Mkfile(const string& str);
	//在当前目录下删去名为str的子目录或文件
	void Delete(const string& str);
	//保存当前目录树结构到文件中
	void Save(const string& fileName);
	//加载当前目录树结构从文件中
	void Load(const string& fileName);
	//查找目录或文件,寻找某一个节点
	void Search(const string& str);
	//将文件夹s1复制到文件夹s2中
	void Cpr(const string& s1, const string& s2);
	//复制文件到相应目录下
	void Cp(const string& s1, const string& s2);
	//获取当前目录的绝对路径
	string getRoute()const { return route; }
protected:
	//更新当前节点到根节点的路径
	void setRoute();
	//判断在相应的目录节点下是否存在要找的子目录或者文件
	bool Judge(CatalogNode* p, const string& name);
	//通过传入的相对路径、绝对路径或者子目录，找到相应的节点
	CatalogNode* FindNode(const string& path);
private:
	CatalogNode* current;	//目录树当前节点
	CatalogNode* root;		//目录树根节点
	string route;			//当前节点到根节点的路径
};
