#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <fstream>
using namespace std;

// spilt the string
vector<string> split(const string &str, const char pattern)
{
	vector<string> res;
	istringstream input(str);
	string temp;
	while (getline(input, temp, pattern))
		res.push_back(temp);
	return res;
}

int state_to_index(const vector<vector<string>> input, string status)
{
	for (int i = 1; i < input.size(); i++)
	{
		if (input[i][0] == status)
			return i;
	}
	return 0;
}

string get_closure(const vector<vector<string>> input, string status, int empty_index)
{
	string result = status;
	vector<string> split_status = split(status, ',');
	for (int i = 0; i < split_status.size(); i++)
	{
		int cur_pos = state_to_index(input, split_status[i]);
		if (input[cur_pos][empty_index] != "Φ" && cur_pos != 0)
		{
			string next_status = input[cur_pos][empty_index].substr(1, input[cur_pos][empty_index].size() - 2);
			vector<string> stats = split(next_status, ',');
			for (auto s : stats)
			{
				if (!count(split_status.begin(), split_status.end(), s)) // Does not include the state s
				{
					result.append(",").append(s);
					split_status.push_back(s);
				}
			}
		}
	}
	return result;
}

bool compare_vector(string src, string dst)
{
	if (src.size() != dst.size())
		return false;
	for (int i = 0; i < src.size(); i++)
	{
		int j = 0;
		for (; j < dst.size(); j++)
		{
			if (src[i] == dst[j])
				break;
		}
		if (j == dst.size())
			return false;
	}
	return true;
}

int NFA_to_DFA()
{
	vector<vector<string>> input;

	string filePath = "./NFA.txt";
	ifstream file;
	file.open(filePath, ios::in);

	if (!file.is_open())
		return 0;

	string strLine;
	vector<vector<string>> all;
	while (getline(file, strLine))
	{
		if (strLine.empty())
			continue;
		vector<string> row = split(strLine, ' ');
		all.push_back(row);
	}
	vector<int> deIndex;
	for (int i = 0; i < all.size(); i++)
	{
		int j = i + 1;
		if (j < all.size() - 1 && all[i][0] == all[j][0] && all[i][1] == all[j][1])
		{
			while (j < all.size() - 1 && all[i][0] == all[j][0] && all[i][1] == all[j][1])
			{
				deIndex.push_back(j);
				all[i][2].append(",").append(all[j][2]);
				j++;
			}
		}
		i = j - 1;
	}
	for (int i = 0; i < deIndex.size(); i++)
		deIndex[i] = deIndex[i] - i;

	for (auto d : deIndex)
		all.erase(all.begin() + d);
	// get all the symbols
	vector<string> syms;
	vector<string> stats;
	for (int i = 0; i < all.size() - 2; i++)
	{
		if (!count(syms.begin(), syms.end(), all[i][1]))
			syms.push_back(all[i][1]);
		stats.push_back(all[i][0]);
	}
	vector<string> symRow;
	symRow.push_back("state");
	for (auto s : syms)
		symRow.push_back(s);
	input.push_back(symRow);
	for (int i = 0; i < all.size() - 2; i++)
	{
		vector<string> outRow;
		outRow.push_back(all[i][0]);
		int k = i;
		while (true)
		{
			if (k < all.size() && all[i][0] == all[k][0])
			{
				for (int j = 0; j < syms.size(); j++)
				{
					if (syms[j] != all[k][1])
						outRow.push_back("Φ");
					else
						break;
				}
				string s = "{" + all[k][2] + "}";
				outRow.push_back(s);
			}
			else
				break;
			k++;
		}
		for (int j = outRow.size(); j < syms.size() + 1; j++)
			outRow.push_back("Φ");
		input.push_back(outRow);
	}
	vector<string> lastRow;
	lastRow.push_back(all[all.size() - 1][1]);
	for (int i = 0; i < syms.size(); i++)
		lastRow.push_back("Φ");

	input.push_back(lastRow);
	// cout << "=================" << endl;
	// for (auto s : input)
	// {
	// 	for (auto ss : s)
	// 		cout << ss << " ";
	// 	cout << endl;
	// }
	int row = input.size();		  // row of the matrix
	int column = input[0].size(); // column of the matrix

	vector<vector<string>> output;
	vector<string> header;
	int empty_index = -1;
	for (int i = 0; i < column; i++)
	{
		if (input[0][i] == "#")
			empty_index = i;
		else
			header.push_back(input[0][i]);
	}
	output.push_back(header);

	// Initialization
	vector<string> status; // status acts as a queue, storing the closure of states
	int cur_pos = 1;
	string init_status = input[cur_pos][0];
	if (empty_index != -1)
	{
		while (input[cur_pos][empty_index] != "Φ")
		{
			init_status.append(",");
			string cur_status = input[cur_pos][empty_index].substr(1, input[cur_pos][empty_index].size() - 2);
			init_status.append(cur_status);
			cur_pos = state_to_index(input, cur_status);
		}
	}
	status.push_back(init_status);
	int cur_index = 0;
	while (cur_index < status.size())
	{
		vector<string> cur_row;
		vector<int> indexes; // records DFA status indexes

		string cur_status = status[cur_index++];
		// cout << cur_status << endl;
		cur_row.push_back(cur_status);

		// update indexes vector
		vector<string> split_stat = split(cur_status, ',');
		for (auto s : split_stat)
			indexes.push_back(state_to_index(input, s));

		for (int i = 1; i < column; i++)
		{
			// Remove the empty set
			if (input[0][i] == "#")
				continue;
			string next_status = "";
			for (auto index : indexes)
			{
				// cout << index << " " << i << " " << input[index][i] << endl;
				if (input[index][i] != "Φ")
				{
					next_status.append(input[index][i].substr(1, input[index][i].size() - 2));
					next_status.append(",");
				}
			}
			if (next_status != "")
			{
				string closure = get_closure(input, next_status.substr(0, next_status.size() - 1), empty_index);
				cur_row.push_back(closure);

				// Determine whether the status already contains closure
				int j = 0;
				for (; j < status.size(); j++)
				{
					if (compare_vector(status[j], closure))
						break;
				}
				if (j == status.size())
					status.push_back(closure);
			}
			else
				cur_row.push_back("Φ");
		}
		output.push_back(cur_row);
	}

	// cout << "output : " << endl;
	// for (auto s : output)
	// {
	// 	for (auto st : s)
	// 		cout << st << "  ";
	// 	cout << endl;
	// }

	map<string, char> map;
	char symbol = 'A';
	for (int i = 1; i < output.size(); i++)
		map.insert({output[i][0], symbol + i - 1});
	vector<vector<char>> DFA;
	for (int i = 1; i < output.size(); i++)
	{
		vector<char> row;
		row.push_back(symbol + i - 1);
		for (int j = 1; j < output[i].size(); j++)
		{
			char value = map[output[i][j]];
			if (value == 0)
			{
				for (auto const &x : map)
				{
					if (compare_vector(x.first, output[i][j]))
					{
						value = x.second;
						break;
					}
				}
			}
			if (value != 0)
				row.push_back(value);
		}
		DFA.push_back(row);
	}
	// cout << "===============================" << endl;
	// for (auto s : DFA)
	// {
	// 	for (auto s1 : s)
	// 		cout << s1 << " ";
	// 	cout << endl;
	// }

	// Output the result to a txt file, and use python to draw a picture according to the result
	ofstream outfile;
	outfile.open("DFA.txt");
	// cout << endl
	// 	 << "DFA : " << endl;
	// for (auto h : header)
	// 	cout << h << "  ";
	// cout << endl;
	vector<string> symbols;
	for (int i = 1; i < header.size(); i++)
		symbols.push_back(header[i]);
	for (int i = 0; i < DFA.size(); i++)
	{
		for (int j = 1; j < DFA[i].size(); j++)
		{
			outfile << DFA[i][0] << ' ' << symbols[j - 1] << ' ' << DFA[i][j] << endl;
			// cout << DFA[i][0] << ' ' << symbols[j - 1] << ' ' << DFA[i][j] << endl;
		}
	}
	outfile << "#" << endl;

	// final state
	// cout << endl
	// 	 << "Final state : " << endl;
	for (int i = 1; i < output.size(); i++)
	{
		if (output[i][0].find(all[all.size() - 1][1]) != string::npos)
		{
			outfile << map[output[i][0]] << " ";
			// cout << map[output[i][0]];
		}
	}
	outfile << endl;
	// cout << endl;
	return 0;
}
