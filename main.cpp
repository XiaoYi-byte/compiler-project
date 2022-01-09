#include "header.h"
#include <string>
#include <iostream>
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
		string path = "input.txt";
		while (_access(path.c_str(), 0) == -1)
		{
			cout << "waiting to input" << endl;
			sleep(1);
		}
		to_NFA(path);
		NFA_to_DFA();
		minimize_DFA();
		DFA_to_code();
		remove(path.c_str());
	}
	return 0;
}