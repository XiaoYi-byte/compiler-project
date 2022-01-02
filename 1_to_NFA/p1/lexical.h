#ifndef _LEXICAL_H_
#define _LEXICAL_H_

#include<string>
#include<iostream>
#include<stack>

using namespace std;

const int MaxNum = 150;//�Զ���������

//��ʼ������ģ���õ��ĺ���
void putin(string&);
bool islegal(string);  //�ж��ַ����ĺ�����
bool legal_char(string);
bool is_letter(char);
bool is_operator(char);
bool legal_match(string);

//���ʽ��+Ԥ����
string addplus(string);

//��׺���ʽת��׺���ʽ
int isp(char);
int osp(char);
string convert(string);


//����NFA�е������Ϣ
//�ڵ�
struct Node
{
    string name;
};

//��
struct Edge
{
    Node start;
    Node end;
    char symbol;  //ת������
};

//�������
struct Cell
{
    Edge edgeset[MaxNum];  //�߼���
    Node startnode;
    Node endnode;
    int edgenumber; //����
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
