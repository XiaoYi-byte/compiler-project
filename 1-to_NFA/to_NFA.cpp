#include <string>
#include <iostream>
#include <stack>
#include <fstream>
using namespace std;

const int MaxNum = 150; //自定义最大边数

//初始化输入模块用到的函数
void putin(string &);
bool islegal(string); //判断字符串的合理性
bool legal_char(string);
bool is_letter(char);
bool is_operator(char);
bool legal_match(string);

//表达式加+预处理
string addplus(string);

//中缀表达式转后缀表达式
int isp(char);
int osp(char);
string convert(string);

//定义NFA中的相关信息
//节点
struct Node
{
	string name;
};

//边
struct Edge
{
	Node start;
	Node end;
	char symbol; //转化符号
};

//各个组分
struct Cell
{
	Edge edgeset[MaxNum]; //边集合
	Node startnode;
	Node endnode;
	int edgenumber; //边数
};

Cell toNFA(string);
Node AddNode();
Cell op_letter(char);
Cell op_plus(Cell, Cell);
Cell op_or(Cell, Cell);
Cell op_close(Cell);

void copyedge(Cell &, Cell);

int show(Cell);

int StateNum = 0;

void putin(string &sentence)
{
	cout << "Please enter the expression:";
	cin >> sentence;
	while (!islegal(sentence))
	{
		cout << "输入的表达式非法，请重新输入：";
		cin >> sentence;
	}
}

bool islegal(string sentence)
{
	int length = sentence.size();
	//保证每个字符是合法的，同时括号要匹配
	if ((!legal_char(sentence)) || (!legal_match(sentence)))
	{
		return false;
	}
	return true;
}

bool is_letter(char c)
{
	if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')))
		return true;
	else
		return false;
}

bool is_operator(char c)
{
	if ((c == '(') || (c == ')') || (c == '|') || (c == '*'))
		return true;
	else
		return false;
}

bool legal_char(string s)
{
	for (int i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if (!is_letter(c) && !is_operator(c))
			return false;
	}
	return true;
}

bool legal_match(string s)
{
	stack<char> STACK;
	for (int i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if (c == '(')
			STACK.push(c);
		if (c == ')')
		{
			if (!STACK.empty() && STACK.top() == '(')
				STACK.pop();
			else
			{
				cout << "无匹配的左括号(" << endl;
				return false;
			}
		}
	}
	if (!STACK.empty())
	{
		cout << "无匹配的右括号)" << endl;
		return false;
	}

	return true;
}

string addplus(string s)
{
	string final;
	for (int i = 0, j = 1; j < s.size(); i++, j++) //给两个监测指针
	{
		if (is_letter(s[i]) && is_letter(s[j]))
			final = final + s[i] + '+'; //两个字母相邻中间加+
		else if (s[i] == '*' && is_letter(s[j]))
			final = final + s[i] + '+';
		else if (s[i] == ')' && is_letter(s[j]))
			final = final + s[i] + '+';
		else if (s[i] == ')' && s[j] == '(')
			final = final + s[i] + '+'; //字母加(情况下中间加+
		else if (s[i] == '*' && s[j] == '(')
			final = final + s[i] + '+';
		else if (is_letter(s[i]) && s[j] == '(')
			final = final + s[i] + '+';

		else
			final = final + s[i];
	}
	final = final + s[s.size() - 1]; //最后一项最后加上

	return final;
}

int isp(char c) //栈内优先级
{
	switch (c)
	{
	case '#':
		return 0;
	case '(':
		return 1;
	case '*':
		return 7;
	case '|':
		return 5;
	case '+':
		return 3;
	case ')':
		return 8; //栈内不会出现右括号？
	default:
		cout << "error" << endl; //理论上不会出现其他非法字符
		return false;
	}
}

int osp(char c)
{
	switch (c)
	{
	case '#':
		return 0;
	case '(':
		return 8;
	case '*':
		return 6; //相同运算符仍不入栈
	case '|':
		return 4;
	case '+':
		return 2;
	case ')':
		return 1; //栈内不会出现右括号？
	default:
		cout << "error" << endl; //理论上不会出现其他非法字符
		return false;
	}
}

string convert(string s)
{
	string final;
	char temp;
	stack<char> sign;
	sign.push('#');
	int length = s.size();
	for (int i = 0; i < length; i++)
	{
		if (is_letter(s[i]))
			final += s[i]; //如果是非运算符直接加入最终字符串
		else
		{
			while (osp(s[i]) < isp(sign.top())) //不符合条件就一直pop
			{
				temp = sign.top();
				sign.pop();
				final += temp;
			}
			if (s[i] != ')')
				sign.push(s[i]);
			else
				sign.pop(); //如果是右括号的话，栈顶一定是左括号，也弹出来
		}
	}
	while (sign.top() != '#') //把符号栈里面可能的运算符都pop出来加到final后面
	{
		temp = sign.top();
		sign.pop();
		final += temp;
	}
	//    cout << final << endl;
	return final;
}

Cell toNFA(string s)
{
	int length = s.size();
	stack<Cell> cstack;
	Cell temp, cell1, cell2;
	for (int i = 0; i < length; i++)
	{
		if (is_letter(s[i]))
			temp = op_letter(s[i]);
		else if (s[i] == '+')
		{
			cell1 = cstack.top();
			cstack.pop();
			cell2 = cstack.top();
			cstack.pop();
			temp = op_plus(cell2, cell1);
		}
		else if (s[i] == '|')
		{
			cell1 = cstack.top();
			cstack.pop();
			cell2 = cstack.top();
			cstack.pop();
			temp = op_or(cell2, cell1);
		}
		else if (s[i] == '*')
		{
			cell1 = cstack.top();
			cstack.pop();
			temp = op_close(cell1);
		}
		else
		{
			cout << "error" << endl;
			//      return false;
		}
		cstack.push(temp);
	}
	temp = cstack.top();
	cstack.pop();
	return temp;
}

Node AddNode()
{
	Node newnode;
	newnode.name = StateNum + 65; //转化成大写字母
	StateNum++;
	return newnode;
}

Cell op_letter(char c)
{
	Cell cell;
	cell.edgenumber = 0;
	Edge edge;

	//首先生成节点
	Node st = AddNode();
	Node en = AddNode();

	//边
	edge.start = st;
	edge.end = en;
	edge.symbol = c;

	//模块
	cell.edgeset[cell.edgenumber++] = edge;
	cell.startnode = cell.edgeset[0].start;
	cell.endnode = cell.edgeset[0].end;

	//   cout << "单字符模块的首结点是：" << cell.startnode.name << endl;
	//  cout << "单字符模块的尾结点是：" << cell.endnode.name << endl;

	return cell;
}

Cell op_plus(Cell c1, Cell c2)
{
	Cell cell;
	cell.edgenumber = 0;
	Edge edge;

	//边的处理
	copyedge(cell, c1);
	copyedge(cell, c2);

	edge.start = c1.endnode; //这里的这种表述和c1.endnode等价吗？
	edge.end = c2.startnode;
	edge.symbol = '#';

	// cell的处理
	cell.startnode = c1.startnode;
	cell.endnode = c2.endnode;

	cell.edgeset[cell.edgenumber++] = edge;

	//   cout << "加法运算的首结点是：" << cell.startnode.name << endl;
	//  cout << "加法运算的尾结点是：" << cell.endnode.name << endl;

	return cell;
}

Cell op_or(Cell c1, Cell c2)
{
	Cell cell;
	cell.edgenumber = 0;
	Edge edge1, edge2, edge3, edge4;

	//首先生成首尾节点
	Node st = AddNode();
	Node en = AddNode();

	//边的处理
	copyedge(cell, c1);
	copyedge(cell, c2);

	edge1.start = st;
	edge1.end = c1.edgeset[0].start;
	edge1.symbol = '#';

	edge2.start = st;
	edge2.end = c2.edgeset[0].start;
	edge2.symbol = '#';

	edge3.start = c1.edgeset[c1.edgenumber - 1].end;
	edge3.end = en;
	edge3.symbol = '#';

	edge4.start = c2.edgeset[c2.edgenumber - 1].end;
	edge4.end = en;
	edge4.symbol = '#';

	// cell的处理
	cell.startnode = st;
	cell.endnode = en;

	cell.edgeset[cell.edgenumber++] = edge1;
	cell.edgeset[cell.edgenumber++] = edge2;
	cell.edgeset[cell.edgenumber++] = edge3;
	cell.edgeset[cell.edgenumber++] = edge4;

	//   cout << "或运算的首结点是：" << cell.startnode.name << endl;
	// cout << "或运算的尾结点是：" << cell.endnode.name << endl;

	return cell;
}

Cell op_close(Cell c)
{
	Cell cell;
	cell.edgenumber = 0;
	Edge edge1, edge2, edge3, edge4;

	//首先生成首尾节点
	Node st = AddNode();
	Node en = AddNode();

	//边的处理
	edge1.start = st;
	edge1.end = c.startnode;
	edge1.symbol = '#';

	edge2.start = st;
	edge2.end = en;
	edge2.symbol = '#';

	edge3.start = c.endnode;
	edge3.end = en;
	edge3.symbol = '#';

	edge4.start = c.endnode;
	edge4.end = c.startnode;
	edge4.symbol = '#';

	copyedge(cell, c);

	// cell的处理
	cell.startnode = st;
	cell.endnode = en;

	cell.edgeset[cell.edgenumber++] = edge1;
	cell.edgeset[cell.edgenumber++] = edge2;
	cell.edgeset[cell.edgenumber++] = edge3;
	cell.edgeset[cell.edgenumber++] = edge4;

	//  cout << "闭包运算的首结点是：" << cell.startnode.name << endl;
	//  cout<< "闭包运算的尾结点是：" << cell.endnode.name << endl;

	return cell;
}

void copyedge(Cell &final, Cell origin)
{
	int fnumber = final.edgenumber;
	int onumber = origin.edgenumber;

	for (int i = 0; i < onumber; i++)
	{
		final.edgeset[fnumber + i] = origin.edgeset[i];
	}
	final.edgenumber = fnumber + onumber;
}

int show(Cell c)
{
	ofstream outfile("f1.txt", ios::out | ios::app);
	if (!outfile.is_open())
	{
		cout << "open error!" << endl;
		return -1;
	}
	for (int i = 0; i < c.edgenumber; i++)
	{
		outfile << c.edgeset[i].start.name << " " << c.edgeset[i].symbol << " " << c.edgeset[i].end.name << endl;
	}
	outfile << "&" << endl;
	outfile << c.startnode.name << " " << c.endnode.name << endl;
	outfile << endl;

	outfile.close();
	return 0;
}

int main()
{
	Cell cell;
	string line;
	//从文件读
	ifstream myfile;
	myfile.open("input.txt");
	if (myfile)
	{
		while (getline(myfile, line))
		{
			while (!islegal(line))
			{
				cout << "illegal expression,error!";
				return -1;
			}
			line = addplus(line); //对初始句子进行加+处理，得到sentence1；
			line = convert(line); //中缀转后缀
			cell = toNFA(line);
			show(cell);
		}
	}
	myfile.close();

	//控制台读
	/*
	 string sentence= "(a|b)*abb";
	 putin(sentence);
	 sentence=addplus(sentence);  //对初始句子进行加+处理，得到sentence1；
	 sentence=convert(sentence);  //中缀转后缀
	 cell=toNFA(sentence);
	 show(cell);
	 */
	return 0;
}
