#include <string>
#include <iostream>
#include <fstream>
#include <io.h>
#include <unistd.h>
using namespace std;

int to_NFA(string path);
int NFA_to_DFA();
int minimize_DFA();
int DFA_to_code();
int main()
{
	while (1)
	{
		string path = "./file/input.txt";
		// ifstream file;
		// file.open(path, ios::in);
		while (_access(path.c_str(), 0) == -1)
		{
			cout << "waiting to input" << endl;
			sleep(1);
		}
		string path1 = "./file/NFA.txt";
		// ifstream file;
		// file.open(path, ios::in);
		if (_access(path1.c_str(), 0) != -1)
			remove(path1.c_str());
		to_NFA(path);
		NFA_to_DFA();
		minimize_DFA();
		DFA_to_code();
		sleep(1);
	}
	return 0;
}