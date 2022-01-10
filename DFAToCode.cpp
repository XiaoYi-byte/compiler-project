#include <iostream>
#include <fstream>
#include <vector>
#include <io.h>
#include <unistd.h>
using namespace std;

bool isSingleSymbol(char ch)
{
	char arr[9] = {'+', '-', '*', '/', '=', '(', ')', ';', ','};
	for (int i = 0; i < 9; i++)
	{
		if (ch == arr[i])
		{
			return true;
		}
	}
	return false;
}
bool maybeDoubleSymbol(char ch)
{
	char arr[3] = {':', '>', '<'};
	for (int i = 0; i < 3; i++)
	{
		if (ch == arr[i])
		{
			return true;
		}
	}
	return false;
}
bool isDoubleSymbol(string str)
{
	string strs[3] = {":=", ">=", "<="};
	for (int i = 0; i < 3; i++)
	{
		if (str == strs[i])
		{
			return true;
		}
	}
	return false;
}

char checkChar(char x)
{
	if (x >= 'a' && x <= 'z' || x >= 'A' && x <= 'Z')
		return 'l';
	else if (x >= '0' && x <= '9')
		return 'd';
	else
		return '0';
};

bool checkDFA(vector<vector<char>> DFA, vector<char> fStates, string Str, int type, ofstream& out)
{

	/*   for (int i = 0; i < fStates.size(); i++) {
		   out << fStates[i] << ' ';
	   }
	   out << endl;
	   for (int i = 0; i < DFA.size(); i++) {
		   for (int j = 0; j < DFA[1].size(); j++) {
			   out << DFA[i][j] << ' ';
		   }
		   out << endl;
	   }*/

	bool isRead = false;
	int row = 1;
	char state = DFA[row][0];
	unsigned int p = 0;
	int errNum = 0;
	while (p < Str.size())
	{
		for (unsigned int i = 1; i < 3; i++)
		{
			if (checkChar(Str[p]) == '0')
			{
				errNum++;
				if (type == 0)
				{
					out << "error1" << ' ';
				}

				break;
			}
			if (checkChar(Str[p]) == DFA[0][i])
			{
				state = DFA[row][i];
				if (state == '#')
				{
					errNum++;
					if (type == 0)
					{
						out << "error2" << ' ';
					}
				}
				break;
			}
		}

		if (errNum > 0)
			break;
		for (unsigned int i = 1; i < 3; i++)
		{
			if (state == DFA[i][0])
			{
				row = i;
			}
		}
		p++;
	}
	if (errNum == 0)
	{
		bool isFinalState = false;
		for (unsigned int i = 0; i < 1; i++)
		{
			if (state == fStates[i])
			{
				isFinalState = true;
				if (type == 0)
				{
					out << "<V," << Str << ">" << ' ';
				}
				if (type == 1)
				{
					out << "<" << Str << ">" << ' ';
				}
				isRead = true;
				break;
			}
		}
		if (!isFinalState)
		{
			if (type == 0)
			{
				out << "error3" << ' ';
			}
		}
	}
	return isRead;
}

int DFA_to_code()
{

	vector<string> KeyWords;
	vector<string> Symbols;

	ifstream file0;
	file0.open("./file/DK.txt");
	char c;
	string str;
	int type = 0;
	file0 >> c;
	if (c == 'K')
	{
		type = 0;
		file0 >> c;
		file0 >> c;
	}
	else
	{
		type = 1;
		file0 >> c;
		file0 >> c;
	}
	while (file0 >> c)
	{
		if (c == '#')
		{
			if (str.size() != 0)
			{
				if (type == 0)
				{
					KeyWords.push_back(str);
				}
				if (type == 1)
				{
					Symbols.push_back(str);
				}
				str.clear();
			}
			file0 >> c;
			if (c == 'K')
			{
				type = 0;
				file0 >> c;
				file0 >> c;
				continue;
			}
			else if (c == 'D')
			{
				type = 1;
				file0 >> c;
				file0 >> c;
				continue;
			}
			else
			{
				break;
			}
		}
		str.push_back(c);
	}
	ifstream file;
	char ch1;
	file.open("./file/DFAmin.txt");

	vector<vector<char>> tagDFA;
	vector<char> ROW;
	vector<char> tagFinalStates;
	ROW.push_back('&');

	if (file.is_open())
	{
		bool matrixFinished = false;
		int p = 0;
		while (file >> ch1)
		{
			if (p == 0 && ch1 == '#')
			{
				matrixFinished = true;
				continue;
			}
			if (!matrixFinished)
			{
				if (ch1 == '`')
				{
					tagDFA.push_back(ROW);
					ROW.clear();
					p = 0;
				}
				else
				{
					ROW.push_back(ch1);
					p++;
				}
			}
			else
			{
				tagFinalStates.push_back(ch1);
			}
		}
	}
	file.close();

	vector<vector<char>> numDFA = {{'&', 'l', 'd'}, {'P', '#', 'Q'}, {'Q', '#', 'Q'}};
	vector<char> numFinalStates = {'Q'};

	/*for (int i = 0; i < tagFinalStates.size(); i++) {
		out << tagFinalStates[i] << ' ';
	}
	out << endl;
	for (int i = 0; i < tagDFA.size(); i++) {
		for (int j = 0; j < tagDFA[1].size(); j++) {
			out << tagDFA[i][j] << ' ';
		}
		out << endl;
	}

	for (int i = 0; i < numFinalStates.size(); i++) {
		out << tagFinalStates[i] << ' ';
	}
	out << endl;
	for (int i = 0; i < numDFA.size(); i++) {
		for (int j = 0; j < numDFA[1].size(); j++) {
			out << numDFA[i][j] << ' ';
		}
		out << endl;
	}*/

	vector<string> StrArray;
	string Str;
	char ch;
	string path = "./file/in.txt";
	while (_access(path.c_str(), 0) == -1)
	{
		cout << "waiting to input1" << endl;
		sleep(1);
	}
	ifstream in;
	in.open(path, ios::in);
	ofstream out;
	out.open("./file/out.txt", ios::out);
	StrArray.clear();
	Str.clear();
	// out << "please input your code:" << endl;
	while (1)
	{
		ch = in.get();
	loop:
		if (ch == ' ')
		{
			if (Str.size() == 0)
				continue;
			else
			{
				StrArray.push_back(Str);
				Str.clear();
				continue;
			}
		}
		if (isSingleSymbol(ch))
		{
			if (Str.size() != 0)
			{
				StrArray.push_back(Str);
				Str.clear();
			}
			Str.push_back(ch);
			StrArray.push_back(Str);
			Str.clear();
			continue;
		}
		if (maybeDoubleSymbol(ch))
		{
			if (Str.size() != 0)
			{
				StrArray.push_back(Str);
				Str.clear();
			}
			Str.push_back(ch);
			ch = in.get();
			Str.push_back(ch);
			if (isDoubleSymbol(Str))
			{
				StrArray.push_back(Str);
				// if(Str.size()!=0){}
				/*Str.push_back(ch);
				StrArray.push_back(Str);*/
				Str.clear();
				continue;
			}
			else
			{
				Str.pop_back();
			}
			if (ch == ' ')
			{
				StrArray.push_back(Str);
				Str.clear();
				continue;
			}
			else
			{
				StrArray.push_back(Str);
				Str.clear();
				goto loop;
			}
		}
		if (ch == '\n')
		{
			if (Str.size() != 0)
			{
				StrArray.push_back(Str);
			}
			break;
		};
		Str.push_back(ch);
	}

	for (int i = 0; i < StrArray.size(); i++)
	{
		bool isRead = false;
		for (int j = 0; j < KeyWords.size(); j++)
		{
			if (StrArray[i] == KeyWords[j])
			{
				isRead = true;
				out << "<" << KeyWords[j] << ">" << ' ';
				break;
			}
		}
		if (isRead)
			continue;
		for (int j = 0; j < Symbols.size(); j++)
		{
			if (StrArray[i] == Symbols[j])
			{
				isRead = true;
				out << "<" << Symbols[j] << ">" << ' ';
				break;
			}
		}
		if (isRead)
			continue;
		if (checkDFA(numDFA, numFinalStates, StrArray[i], 1, out))
		{
			continue;
		}
		checkDFA(tagDFA, tagFinalStates, StrArray[i], 0, out);
	}

	// out << endl
	// 	 << "press 0 to quit." << endl;
	// out << "press others keys to continue." << endl;
	// char s = in.get();
	// if (s == '0')
	// 	break;
	in.close();
	out.close();
	remove("./file/in.txt");
	return 0;
}