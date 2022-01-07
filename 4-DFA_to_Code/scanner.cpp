#include<iostream>
#include<vector>
using namespace std;
char checkChar(char x) {
	if (x >= 'a' && x <= 'z' || x >= 'A' && x <= 'Z')return 'l';
	else if (x >= '0' && x <= '9')return 'd';
	else return '0';
};
int main(){
	char DFA[3][3] = {
	's', 'l', 'd', 
	'P', 'Q', '0', 
	'Q', 'Q', 'Q', 
	};
	char fStates[1] = {'Q', };
    while (1) {
        vector<char> Str;
        cout << "please input the variable:" << endl;
        char ch;
        cin >> ch;
        while (ch != '\n'){
            Str.push_back(ch);
            ch = cin.get();
        }
        int row = 1;
        char state = DFA[row][0];
        unsigned int p = 0;
        int errNum = 0;
        while (p < Str.size()) {
            for (unsigned int i = 1; i < 3; i++) {
                if (checkChar(Str[p]) == '0') {
                    errNum++;
                    cout << "unexpected characters!" << endl;
                    break;
                }
                if (checkChar(Str[p]) == DFA[0][i]) {
                    state = DFA[row][i];
                    if (state == '0') {
                        errNum++;
                        cout << "error" << endl;
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
                    cout << "reach the final state: "<<fStates[i]<< endl;
                    cout << "<V,";
                    for (unsigned int j = 0; j < Str.size(); j++) {
                        cout << Str[j];
                    }
                    cout << ">" << endl;
                    break;
                }
            }
            if (!isFinalState) {
                cout << "fail to reach the final state" << endl;
            }
        }
        cout << "press other keys to continue." << endl;
        cout << "press 0 to quit" << endl;
        char opt;
        opt = cin.get();
        if (opt == '0')break;
    }
}