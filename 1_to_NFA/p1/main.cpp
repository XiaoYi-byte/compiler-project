#include"lexical.h"
#include<string>
#include<iostream>

using namespace std;


int main()
{
    Cell cell;
    string sentence= "(a|b)*abb";
    putin(sentence);
    sentence=addplus(sentence);  //�Գ�ʼ���ӽ��м�+�����õ�sentence1��
    sentence=convert(sentence);  //��׺ת��׺
    cell=toNFA(sentence);
    show(cell);

    return 0;

}