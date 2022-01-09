#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define max 100

struct edge
{
	string first;	  //边的初始结点
	string condition; //边上的条件
	string last;	  //边的终点
};
int N;			  // NFA的边数
string part[max]; //分割子集

string move(string collection, char ch, edge *b) //状态集合I的a弧转换
{
	int i, j;
	string s = "";
	for (i = 0; i < collection.length(); i++)
	{
		for (j = 0; j < N; j++)
		{
			if (b[j].first[0] == collection[i] && b[j].condition[0] == ch)
				s = s + b[j].last;
		}
	}
	if (s == "")
		return "&";
	else
		return s;
}

bool isexist(string s, string d) //判断子串是否存在在某一集合
{
	if (d != "" && 0 <= d.find(s) && d.find(s) <= d.length() - 1)
		return 1;
	else
		return 0;
}

int divide(edge *b, string change) //分割子集法进行DFA的最小化
{
	int x, m, flag = 2, flag0, i, j;
	//最初只有初始和最终状态
	string ss, part0[max];
	flag0 = flag;
	for (x = 0; x < change.length(); x++)
	{
		for (m = 0; m < flag0; m++)
		{
			for (i = 0; i < part[m].length(); i++)
			{
				ss = move(part[m].substr(i, 1), change[x], b);
				for (j = 0; j < flag; j++)
				{
					if (isexist(ss, part[j]))
						part0[j] = part0[j] + part[m].substr(i, 1);
					if (ss == "&")
					{
						part0[flag] = part0[flag] + part[m].substr(i, 1);
						break;
					}
				}
			}
			for (j = 0; j <= flag; j++)
			{
				if (part0[j] != "" && part0[j] != part[m])
				{
					part[flag++] = part0[j];
					part0[j] = "";
					part[m] = "";
				}
				else
					part0[j] = "";
			}
		}
		flag0 = flag;
	}
	return flag;
}

//去掉空格
void trimAllSpace(string &strSrc)
{
	string delem = " \t"; //空字符: 空格或者tab键
	string::size_type pos = strSrc.find_first_of(delem, 0);
	while (pos != string::npos)
	{
		strSrc.erase(pos, 1);
		pos = strSrc.find_first_of(delem, 0);
	}
	return;
}

int minimize_DFA()
{
	int i, j, flag, x;
	string Condition = ""; //边上的条件
	string ss;
	string tmp = "";
	edge *b = new edge[max];
	edge *res = new edge[max]; //最小化结果的储存
	ifstream fin;
	ofstream fout;
	fin.open("DFA.txt");
	fout.open("DFAmin.txt");
	if (!fin.is_open() || !fout.is_open())
	{
		cout << "cannot open the file" << endl;
	}
	for (i = 0; i < max; i++)
	{
		fin >> b[i].first;
		if (b[i].first == "#")
			break;
		fin >> b[i].condition >> b[i].last;
		if (!isexist(b[i].first, tmp))
		{
			tmp += b[i].first;
		}
		if (!isexist(b[i].last, tmp))
		{
			tmp += b[i].last;
		}
		if (!isexist(b[i].condition, Condition))
		{
			Condition += b[i].condition;
		}
	}
	N = i;
	char buffer[1021] = {0};
	while (fin.getline(buffer, sizeof(buffer)))
	{
		getline(fin, part[1]);
	}
	trimAllSpace(part[1]);	//终态
	string final = part[1]; //记录最小化之前的终态
	while (tmp.find(part[1]) < tmp.length())
	{
		tmp.erase(tmp.find(part[1]), part[1].size());
	}
	part[0] = tmp; //起始状态
	flag = divide(b, Condition);
	char letters[max];
	char letter = 'P';
	string newFinal = "";
	int resCnt = 0;
	for (i = 0; i < flag; i++)
	{
		if (part[i] != "")
		{
			resCnt++;
			letters[i] = letter;
			++letter;
			if (isexist(part[i], final))
			{
				newFinal += letters[i];
			}
		}
	}
	int edgeCnt = 0;
	for (i = 0; i < flag; i++)
	{
		for (j = 0; j < Condition.length(); j++)
		{
			ss = move(part[i], Condition[j], b);
			if (part[i] != "" && ss != "&")
			{
				res[edgeCnt].first = letters[i];
				res[edgeCnt].condition = Condition[j];
			}
			for (x = 0; x < flag; x++)
			{
				if (isexist(ss.substr(0, 1), part[x]))
				{
					res[edgeCnt++].last = letters[x];
				}
			}
		}
	}
	for (i = 0; i < Condition.size(); i++)
	{
		if (i != Condition.length() - 1)
			fout << Condition[i] << " ";
		else
			fout << Condition[i] << " `" << endl;
	}
	string output[resCnt][Condition.length() + 1];
	int cnt = 0;
	for (i = 0; i < edgeCnt; i++)
	{
		output[cnt][0] = res[i].first;
		int pos = Condition.find(res[i].condition);
		output[cnt][pos + 1] = res[i].last;
		if (i < edgeCnt - 1 && res[i].first != res[i + 1].first)
		{
			for (int l = 0; l < Condition.length() + 1; l++)
			{
				output[cnt][l] = output[cnt][l] == "" ? "#" : output[cnt][l];
			}
			cnt++;
		}
	}
	for (i = 0; i < resCnt; i++)
	{
		for (j = 0; j < Condition.length() + 1; j++)
		{
			if (j != Condition.length())
				fout << output[i][j] << " ";
			else
				fout << output[i][j] << " `" << endl;
		}
	}
	fout << "#"<< endl;
	fout << newFinal;
	fin.close();
	fout.close();
	return 0;
}
