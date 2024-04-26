#include"CatalogTree.h"
#include<bits/stdc++.h>
using namespace std;

CatalogTree::CatalogTree()
{
	root = new CatalogNode("C:");
	current = root;
	setRoute();
}

void CatalogTree::Dir()
{
	CatalogNode* p = current;
	for (auto it = p->next.begin(); it != p->next.end(); it++) {
		if (!(*it)->isFile) {
			cout << ">/" << (*it)->name << "/\n";
		}
	}
	for (auto it = p->next.begin(); it != p->next.end(); it++) {
		if ((*it)->isFile) {
			cout << ">*" << (*it)->name << "\n";
		}
	}
}

void CatalogTree::Cd()
{
	cout << ">" << route << "/\n";
}

void CatalogTree::CdUp()
{
	if (current == root)return;
	current = current->father;
	setRoute();
}

void CatalogTree::CdTo(const string& str)
{
	CatalogNode* p = FindNode(str);
	if (p != nullptr) {
		current = p;
		setRoute();
	}
	else {
		cout << ">The path is error！" << "\n";
	}
}
void CatalogTree::Mkdir(const string& str)
{
	for (auto it = current->next.begin(); it != current->next.end(); it++) {
		if ((*it)->name == str&&!(*it)->isFile) {
			cout << ">The item " << str << " is already exited." << "\n";
			return;
		}
	}
	CatalogNode* p=new CatalogNode(str,current);
	current->next.push_back(p);

	sort(current->next.begin(), current->next.end());

	cout << ">create item " << str << " is success." << "\n";
}

void CatalogTree::Mkfile(const string& str)
{
	for (auto it = current->next.begin(); it != current->next.end(); it++) {
		if ((*it)->name == str&&(*it)->isFile) {
			cout << ">The file " << str << " is already exited." << "\n";
			return;
		}
	}
	CatalogNode* p = new CatalogNode(str, current, true);
	current->next.push_back(p);
	cout << ">create file " << str << " is success." << "\n";
}

void CatalogTree::Delete(const string& str)
{
	for (auto it = current->next.begin(); it != current->next.end(); it++) {
		if ((*it)->name == str) {
			if (!(*it)->isFile) {
				cout << ">delete item " << str << " is success." << "\n";
			}
			else {
				cout << ">delete file " << str << " is success." << "\n";
			}
			current->next.erase(it);
			break;
		}
	}
}

void CatalogTree::Save(const string& fileName)
{
	ofstream fout(fileName+".txt", ios::out);

	stack<CatalogNode*>s;

	for (auto it = root->next.begin(); it != root->next.end(); it++) {
		if (!(*it)->isFile) {
			fout << "mkdir " << (*it)->name << "\n";
			if ((*it)->next.size() != 0) {//存在子目录或者文件
				s.push(*it);
			}
		}
		else {
			fout << "mkfile " << (*it) << "\n";
		}
	}

	CatalogNode* p = root;
	while (!s.empty()) {
		bool flag = false;

		for (auto it = p->next.begin(); it != p->next.end(); it++) {
			if ((*it) == s.top()) {
				p = s.top();
				flag = true;
				break;
			}
		}
		if (!flag) {//返回父目录
			p = p->father;
			fout << "cd .." << "\n";
		}
		else {//向下创建子目录或文件
			CatalogNode* q=s.top();
			s.pop();

			fout << "cd " << q->name << "\n";

			for (auto it = q->next.begin(); it != q->next.end(); it++) {
				if (!(*it)->isFile) {
					fout << "mkdir " << (*it)->name << "\n";
					if ((*it)->next.size() != 0) {//存在子目录或者文件
						s.push(*it);
					}
				}
				else {
					fout << "mkfile " << (*it)->name << "\n";
				}
			}
		}
	}
	fout << "quit" << "\n";
	fout.close();
}

void CatalogTree::Load(const string& fileName)
{
	root = new CatalogNode("C:");
	current = root;

	ifstream fin(fileName+".txt", ios::in);
	while (1) {
		string s;
		getline(fin, s); 
		if (s == "quit") {
			break;
		}
		else if (s == "cd ..") {
			CdUp();
			continue;
		}
		else if (s.substr(0, 3) == "cd ") {
			CdTo(s.substr(3));
			continue;
		}
		else if (s.substr(0, 6) == "mkdir ") {
			Mkdir(s.substr(6));
			continue;
		}
		else if (s.substr(0.7) == "mkfile ") {
			Mkfile(s.substr(7));
			continue;
		}
	}
	current = root;
	fin.close();
}

void CatalogTree::Search(const string& str)
{
	queue<CatalogNode*>q;
	CatalogNode* p = root;

	q.push(p);
	while (!q.empty()) {
		
		CatalogNode *node= q.front();
		q.pop();

		for (auto it = node->next.begin(); it != node->next.end(); it++) {
			if ((*it)->name == str) {
				p = node;
				if (p == root) {
					if ((*it)->isFile) {
						cout << ">" << p->name << "*" << str << "\n";
					}
					else {
						cout << ">" << p->name << "/" << str << "/\n";
					}
				}
				else {
					if ((*it)->isFile) {
						string path = "/*"+str;

						while (p != root) {
							path = '/' + p->name + path;
							p = p->father;
						}
						path = root->name + path;
						cout << ">" << path << "\n";
					}
					else {
						string path = "/" + str + "/";

						while (p != root) {
							path = '/' + p->name + path;
							p = p->father;
						}
						path = root->name + path;
						cout << ">" << path << "\n";
					}
				}	
			}
			if ((*it)->next.size() != 0) {
				q.push(*it);
			}
		}
	}
}

void CatalogTree::Cpr(const string& s1, const string& s2)
{
	CatalogNode* p = FindNode(s1);
	CatalogNode* q = FindNode(s2);
	for (auto it = p->next.begin(); it != p->next.end(); it++) {
		CatalogNode* temp = *it;
		if (Judge(q, s1)) {
			temp->name = temp->name + '1';
		}
		q->next.push_back(temp);
	}
	sort(q->next.begin(), q->next.end());
}

void CatalogTree::Cp(const string& name, const string& str)
{
	bool flag = false;
	for (auto it = current->next.begin(); it != current->next.end(); it++)
	{
		if ((*it)->name == name&&(*it)->isFile)
		{
			flag = true;
			break;
		}
	}
	if (flag)
	{
		string s = str;
		CatalogNode* p = root;

		while (1)
		{
			if (s.rfind('/') == string::npos)            //s中是否存在'/'
				break;
			int len = s.length();
			string s1 = s.substr(0, str.find('/', 0));             //得到该目录下的下一个子目录的名称
			s = s.substr(s.find('/', 0) + 1, len);

			for (auto it = p->next.begin(); it != p->next.end(); it++)
			{
				if ((*it)->name == s1 && !(*it)->isFile)
				{
					p = *it;
					break;
				}
			}
		}
		for (auto it = p->next.begin(); it != p->next.end(); it++)            //将最后一个子目录名称进行读取即可
		{
			if ((*it)->name == s&&!(*it)->isFile)
			{
				p = *it;
				break;
			}
		}
		if (Judge(p, name)) {
			CatalogNode* node = new CatalogNode(name+"1", p, true);
			p->next.push_back(node);
		}
		else {
			CatalogNode* node = new CatalogNode(name + "1", p, true);
			p->next.push_back(node);
		}
	}
	else {
		cout << ">The file name or the path is error.\n";
	}
}

void CatalogTree::setRoute()
{
	string path = "";

	CatalogNode* p = current;
	while (p != root) {
		path = '/' + p->name + path;
		p = p->father;
	}
	route = root->name + path;
}

bool CatalogTree::Judge(CatalogNode* p, const string& name)
{
	for (auto it = p->next.begin(); it != p->next.end(); it++) {
		if ((*it)->name == name) {
			return true;
		}
	}
	return false;
}

CatalogNode* CatalogTree::FindNode(const string& path)
{
	CatalogNode* p = nullptr;

	int len = path.length();
	string s = path;

	if (s.rfind('/') == s.npos) {//path是当前目录的子目录
		p = current;

		for (auto it = p->next.begin(); it != p->next.end(); it++) {
			if ((*it)->name == s&&!(*it)->isFile) {
				p = *it;
				break;
			}
		}
	}
	else if(s.substr(0,2)==root->name) {//path是一个绝对路径
		p = root;
		while (1) {
			if (s.rfind('/') == s.npos)break;//已经到最小一级目录了

			len = s.length();
			string s1 = s.substr(0, s.find('/', 0));
			s = s.substr(s.find('/', 0)+1, len);

			for (auto it = p->next.begin(); it != p->next.end(); it++) {
				if ((*it)->name == s1 && !(*it)->isFile) {
					p = *it;
					break;
				}
			}
		}
		//在次一级目录文件夹里找到最小一级目录
		for (auto it = p->next.begin(); it != p->next.end(); it++) {
			if ((*it)->name == s && !(*it)->isFile) {
				p = *it;
				break;
			}
		}
	}
	else {//path是当前目录下的一个相对路径，即子目录的子目录
		s = path.substr(1, len-2);
		p = current;
		while (1) {
			if (s.rfind('/') == s.npos)break;//已经到最小一级目录了

			len = s.length();
			string s1 = s.substr(0, s.find('/', 0));
			s = s.substr(s.find('/', 0)+1, len);

			for (auto it = p->next.begin(); it != p->next.end(); it++) {
				if ((*it)->name == s1) {
					p = *it;
					break;
				}
			}
		}
		//在最小级目录文件夹里继续寻找
		for (auto it = p->next.begin(); it != p->next.end(); it++) {
			if ((*it)->name == s) {
				p = *it;
				break;
			}
		}
	}
	if (p==current||p == root||p==nullptr) {
		return nullptr;
	}
	else{
		return p;
	}
}
