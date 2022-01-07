#include<iostream>
#include<vector>
#include<string>
#include<fstream>
using namespace std;

void inputDFA(vector<vector<char>>& DFA) {
	int rows, cols;
	cout << "please input the amount of rows and columns:" << endl;
	cin >> rows >> cols;
	cout << "please input a state-transition matrix:" << endl;
	char x;
	for (int i = 0; i < rows; i++) {
		vector<char> row;
		for (int j = 0; j < cols; j++) {
			cin >> x;
			row.push_back(x);
		}
		DFA.push_back(row);
	}
}

void inputFinalStates(vector<vector<char>>& DFA, vector<char>& fStates) {
	cout << "please input the final states:" << endl;
	char s;
	cin >> s;
	while (s != '\n') {
		if (s != ' ') {
			bool isInDFA = false;
			for (int i = 1; i < DFA.size(); i++) {
				if (s == DFA[i][0]) {
					isInDFA = true;
					fStates.push_back(s);
					cout << s << " is set as a final state." << endl;
					break;
				}
			}
			if (!isInDFA) {
				cout << "the input state " << s << " isn't in the state-transition matrix" << endl;
			}
		}
		s = cin.get();
	}
	cout << "the lexical analyzer has been created!" << endl;
}

void generateCode(vector<vector<char>>& DFA, vector<char>& fStates) {
	ofstream fout;
	fout.open("scanner.cpp");
	fout << "#include<iostream>" << endl;
	fout << "#include<vector>" << endl;
	fout << "using namespace std;" << endl;

	fout << "char checkChar(char x) {" << endl;
	fout << "	if (x >= 'a' && x <= 'z' || x >= 'A' && x <= 'Z')return 'l';" << endl;
	fout << "	else if (x >= '0' && x <= '9')return 'd';" << endl;
	fout << "	else return '0';" << endl;
	fout << "};" << endl;

	fout << "int main(){" << endl;

	fout << "	char DFA[" << DFA.size() << "][" << DFA[0].size() << "] = {" << endl;
	for (int i = 0; i < DFA.size(); i++) {
		fout << "	";
		for (int j = 0; j < DFA[0].size(); j++) {
			fout << "'" << DFA[i][j] << "', ";
		}
		fout << endl;
	}
	fout << "	};" << endl;

	fout << "	char fStates[" << fStates.size() << "] = {";
	for (int i = 0; i < fStates.size(); i++) {
		fout << "'" << fStates[i] << "', ";
	}
	fout << "};" << endl;

	fout << "    while (1) {" << endl;
	fout << "        vector<char> Str;" << endl;
	fout << "        cout << \"please input the variable:\" << endl;" << endl;
	fout << "        char ch;" << endl;
	fout << "        cin >> ch;" << endl;
	fout << "        while (ch != '\\n'){" << endl;
	fout << "            Str.push_back(ch);" << endl;
	fout << "            ch = cin.get();" << endl;
	fout << "        }" << endl;
	fout << "        int row = 1;" << endl;
	fout << "        char state = DFA[row][0];" << endl;
	fout << "        unsigned int p = 0;" << endl;
	fout << "        int errNum = 0;" << endl;
	fout << "        while (p < Str.size()) {" << endl;
	fout << "            for (unsigned int i = 1; i < " << DFA[0].size() << "; i++) {" << endl;
	fout << "                if (checkChar(Str[p]) == '0') {" << endl;
	fout << "                    errNum++;" << endl;
	fout << "                    cout << \"unexpected characters!\" << endl;" << endl;
	fout << "                    break;" << endl;
	fout << "                }" << endl;
	fout << "                if (checkChar(Str[p]) == DFA[0][i]) {" << endl;
	fout << "                    state = DFA[row][i];" << endl;
	fout << "                    if (state == '0') {" << endl;
	fout << "                        errNum++;" << endl;
	fout << "                        cout << \"error\" << endl;" << endl;
	fout << "                    }" << endl;
	fout << "                    break;" << endl;
	fout << "                }" << endl;
	fout << "            }" << endl;
	fout << "            if (errNum > 0)break;" << endl;
	fout << "            for (unsigned int i = 1; i < " << DFA.size() << "; i++) {" << endl;
	fout << "                if (state == DFA[i][0]) {" << endl;
	fout << "                    row = i;" << endl;
	fout << "                }" << endl;
	fout << "            }" << endl;
	fout << "            p++;" << endl;
	fout << "        }" << endl;
	fout << "        if (errNum == 0) {" << endl;
	fout << "            bool isFinalState = false;" << endl;
	fout << "            for (unsigned int i = 0; i < " << fStates.size() << "; i++) {" << endl;
	fout << "                if (state == fStates[i]) {" << endl;
	fout << "                    isFinalState = true;" << endl;
	fout << "                    cout << \"reach the final state: \"<<fStates[i]<< endl;" << endl;
	fout << "                    cout << \"<V,\";" << endl;
	fout << "                    for (unsigned int j = 0; j < Str.size(); j++) {" << endl;
	fout << "                        cout << Str[j];" << endl;
	fout << "                    }" << endl;
	fout << "                    cout << \">\" << endl;" << endl;
	fout << "                    break;" << endl;
	fout << "                }" << endl;
	fout << "            }" << endl;
	fout << "            if (!isFinalState) {" << endl;
	fout << "                cout << \"fail to reach the final state\" << endl;" << endl;
	fout << "            }" << endl;
	fout << "        }" << endl;
	fout << "        cout << \"press other keys to continue.\" << endl;" << endl;
	fout << "        cout << \"press 0 to quit\" << endl;" << endl;
	fout << "        char opt;" << endl;
	fout << "        opt = cin.get();" << endl;
	fout << "        if (opt == '0')break;" << endl;
	fout << "    }" << endl;
	fout << "}";
}

int main() {
	vector<vector<char>> DFA;
	inputDFA(DFA);

	vector<char> fStates;
	inputFinalStates(DFA, fStates);

	generateCode(DFA, fStates);

	return 0;
}