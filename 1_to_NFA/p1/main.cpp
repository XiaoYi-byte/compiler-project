#include"lexical.h"
#include<string>
#include<iostream>

using namespace std;


int main()
{
    Cell cell;
    string sentence= "(a|b)*abb";
    putin(sentence);
    sentence=addplus(sentence);  //对初始句子进行加+处理，得到sentence1；
    sentence=convert(sentence);  //中缀转后缀
    cell=toNFA(sentence);
    show(cell);

    return 0;

}