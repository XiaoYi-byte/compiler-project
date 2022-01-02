#ifndef _LEXICAL_H_
#define _LEXICAL_H_

#include<string>
#include<iostream>
#include<stack>

using namespace std;

const int MaxNum = 150;//自定义最大边数

//初始化输入模块用到的函数
void putin(string&);
bool islegal(string);  //判断字符串的合理性
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
    char symbol;  //转化符号
};

//各个组分
struct Cell
{
    Edge edgeset[MaxNum];  //边集合
    Node startnode;
    Node endnode;
    int edgenumber; //边数
};

Cell toNFA(string);
Node AddNode();
Cell op_letter(char);
Cell op_plus(Cell,Cell);
Cell op_or(Cell,Cell);
Cell op_close(Cell);

void copyedge(Cell&,Cell);

void show(Cell);

#endif // !_LEXICAL_H_
