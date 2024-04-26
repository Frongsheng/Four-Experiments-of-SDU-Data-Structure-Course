#include<iostream>
#include"CatalogTree.h"
using namespace std;

int main() {
	CatalogTree catalog;
	string command;
	while (1) {
		string route = catalog.getRoute();
		cout << route << "> ";

		getline(cin, command);

		if (command == "quit") {
			break;
		}
		else if (command == "dir") {
			catalog.Dir();
			continue;
		}
		else if (command == "cd") {
			catalog.Cd();
			continue;
		}
		else if (command == "cd ..") {
			catalog.CdUp();
			continue;
		}
		else if (command.substr(0, 3) == "cd ") {
			catalog.CdTo(command.substr(3));
			continue;
		}
		else if (command.substr(0,6) == "mkdir ") {
			catalog.Mkdir(command.substr(6));
			continue;
		}
		else if (command.substr(0, 7) == "mkfile ") {
			catalog.Mkfile(command.substr(7));
			continue;
		}
		else if (command.substr(0, 7) == "delete ") {
			catalog.Delete(command.substr(7));
			continue;
		}
		else if (command.substr(0, 5) == "save ") {
			catalog.Save(command.substr(5));
			continue;
		}
		else if (command.substr(0, 5) == "load ") {
			catalog.Load(command.substr(5));
			continue;
		}
		else if (command.substr(0, 7) == "search ") {
			catalog.Search(command.substr(7));
			continue;
		}
		else if (command.substr(0, 3) == "cp ") {
			int len = command.length();

			string s1 = command.substr(3, command.find(' ', 0));
			string s2 = command.substr(command.find(' ', 0) + 1, len);

			catalog.Cp(s1, s2);
			continue;
		}
		else if (command.substr(0, 6) == "cp -r ") {
			int len = command.length();

			string s1 = command.substr(6, command.find(' ', 0));
			string s2 = command.substr(command.find(' ', 0) + 1, len);

			catalog.Cpr(s1, s2);
			continue;
		}
		else {
			cout << "Unknow command.Please type correct command, or type 'quit' to exit." << endl;
		}
	}
	return 0;
}