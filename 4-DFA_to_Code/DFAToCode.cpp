#include<iostream>
#include<fstream>
#include<vector>
using namespace std;

bool isSingleSymbol(char ch) {
    char arr[9] = { '+', '-', '*', '/', '=', '(', ')', ';', ',' };
    for (int i = 0; i < 9; i++) {
        if (ch == arr[i]) {
            return true;
        }
    }
    return false;
}
bool maybeDoubleSymbol(char ch) {
    char arr[3] = { ':','>','<' };
    for (int i = 0; i < 3; i++) {
        if (ch == arr[i]) {
            return true;
        }
    }
    return false;
}
bool isDoubleSymbol(string str) {
    string strs[3] = { ":=",">=","<=" };
    for (int i = 0; i < 3; i++) {
        if (str == strs[i]) {
            return true;
        }
    }
    return false;
}

char checkChar(char x) {
    if (x >= 'a' && x <= 'z' || x >= 'A' && x <= 'Z')return 'l';
    else if (x >= '0' && x <= '9')return 'd';
    else return '0';
};

bool checkDFA(vector<vector<char>> DFA, vector<char> fStates, string Str ,int type) {

 /*   for (int i = 0; i < fStates.size(); i++) {
        cout << fStates[i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < DFA.size(); i++) {
        for (int j = 0; j < DFA[1].size(); j++) {
            cout << DFA[i][j] << ' ';
        }
        cout << endl;
    }*/

    bool isRead = false;
    int row = 1;
    char state = DFA[row][0];
    unsigned int p = 0;
    int errNum = 0;
    while (p < Str.size()) {
        for (unsigned int i = 1; i < 3; i++) {
           if (checkChar(Str[p]) == '0') {
                errNum++;
                if (type == 0) {
                    cout << "error" << endl;
                }
                
                break;
            }
            if (checkChar(Str[p]) == DFA[0][i]) {
                state = DFA[row][i];
                if (state == '#') {
                    errNum++;
                    if (type == 0) {
                        cout << "error" << ' ';
                    }
                }
                break;
            }
        }

        if (errNum > 0)break;
        for (unsigned int i = 1; i < 3; i++) {
            if (state == DFA[i][0]) {
                row = i;
            }
        }
        p++;
    }
    if (errNum == 0) {
        bool isFinalState = false;
        for (unsigned int i = 0; i < 1; i++) {
            if (state == fStates[i]) {
                isFinalState = true;
                if (type == 0) {
                    cout << "<V," << Str << ">" << ' ';
                }
                if (type == 1) {
                    cout << "<" << Str << ">"<<' ';
                }
                isRead = true;
                break;
            }
        }
        if (!isFinalState) {
            if (type == 0) {
                cout << "error" << ' ';
            }
        }
    }
    return isRead;
}

int DFA_to_code() {
    vector<string> KeyWords = { "PROGRAM","BEGIN","END","CONST","VAR","WHILE","DO","IF","THEN" };
    vector<string> Symbols = { ":","+","-","*","/",":=","=","<>",">",">=","<","<=","(",")",";","," };
    ifstream file;
    char ch1;
    file.open("DFAmin.txt");

    vector<vector<char>> tagDFA;
    vector<char> ROW;
    vector<char> tagFinalStates;
    ROW.push_back('&');
    
    if (file.is_open()) {
        bool matrixFinished = false;
        int p = 0;
        while (file >> ch1) {
            if (p == 0 && ch1 == '#') {
                matrixFinished = true;
                continue;
            }
            if (!matrixFinished) {
                if (ch1 == '`') {
                    tagDFA.push_back(ROW);
                    ROW.clear();
                    p = 0;
                }
                else {
                    ROW.push_back(ch1);
                    p++;
                }
            }
            else {
                tagFinalStates.push_back(ch1);
            }
        }
    }
    else {
        cout << "file error" << endl;
    }
    file.close();

    vector<vector<char>> numDFA = { {'&','l','d'},{'P','#','Q'},{'Q','#','Q'} };
    vector<char> numFinalStates = { 'Q' };

   
    

    /*for (int i = 0; i < tagFinalStates.size(); i++) {
        cout << tagFinalStates[i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < tagDFA.size(); i++) {
        for (int j = 0; j < tagDFA[1].size(); j++) {
            cout << tagDFA[i][j] << ' ';
        }
        cout << endl;
    }

    for (int i = 0; i < numFinalStates.size(); i++) {
        cout << tagFinalStates[i] << ' ';
    }
    cout << endl;
    for (int i = 0; i < numDFA.size(); i++) {
        for (int j = 0; j < numDFA[1].size(); j++) {
            cout << numDFA[i][j] << ' ';
        }
        cout << endl;
    }*/

    vector<string> StrArray;
    string Str;
    char ch;
    while (1) {
        StrArray.clear();
        Str.clear();
        cout << "please input your code:" << endl;
        while (1)
        {
            ch = cin.get();
        loop:
            if (ch == ' ') {
                if (Str.size() == 0)continue;
                else
                {
                    StrArray.push_back(Str);
                    Str.clear();
                    continue;
                }
            }
            if (isSingleSymbol(ch)) {
                if (Str.size() != 0) {
                    StrArray.push_back(Str);
                    Str.clear();
                }
                Str.push_back(ch);
                StrArray.push_back(Str);
                Str.clear();
                continue;
            }
            if (maybeDoubleSymbol(ch)) {
                if (Str.size() != 0) {
                    StrArray.push_back(Str);
                    Str.clear();
                }
                Str.push_back(ch);
                ch = cin.get();
                Str.push_back(ch);
                if (isDoubleSymbol(Str)) {
                    StrArray.push_back(Str);
                    //if(Str.size()!=0){}
                    /*Str.push_back(ch);
                    StrArray.push_back(Str);*/
                    Str.clear();
                    continue;
                }
                else {
                    Str.pop_back();
                }
                if (ch == ' ') {
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
            if (ch == '\n') {
                if (Str.size() != 0) {
                    StrArray.push_back(Str);
                }
                break;
            };
            Str.push_back(ch);
        }

        for (int i = 0; i < StrArray.size(); i++) {
            bool isRead = false;
            for (int j = 0; j < KeyWords.size(); j++)
            {
                if (StrArray[i] == KeyWords[j]) {
                    isRead = true;
                    cout << "<" << KeyWords[j] << ">" << ' ';
                    break;
                }
            }
            if (isRead)continue;
            for (int j = 0; j < Symbols.size(); j++) {
                if (StrArray[i] == Symbols[j]) {
                    isRead = true;
                    cout << "<" << Symbols[j] << ">" << ' ';
                    break;
                }
            }
            if (isRead)continue;
            if (checkDFA(numDFA, numFinalStates, StrArray[i],1)) {
                continue;
            }
            checkDFA(tagDFA, tagFinalStates, StrArray[i],0);
        }

        cout << endl<< "press 0 to quit." << endl;
        cout << "press others keys to continue." << endl;
        char s = cin.get();
        if (s == '0')break;
    }

    


}