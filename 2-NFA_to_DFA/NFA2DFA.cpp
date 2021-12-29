#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <algorithm>
#include <map>
#include<fstream>
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
		while (input[cur_pos][empty_index] != "Φ")
		{
			string next_status = input[cur_pos][empty_index].substr(1, input[cur_pos][empty_index].size() - 2);
			if (!count(split_status.begin(), split_status.end(), next_status)) // Does not include the state s
			{
				result.append(",");
				result.append(next_status);
				split_status.push_back(next_status);
			}
			cur_pos = state_to_index(input, next_status);
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

int main()
{
	const vector<vector<string>> input = {
		{"state", "a", "b", "ε"},
		{"X", "Φ", "Φ", "{5}"},
		{"1", "{3}", "{4}", "Φ"},
		{"2", "Φ", "Φ", "{6}"},
		{"3", "{2}", "Φ", "Φ"},
		{"4", "Φ", "{1,2}", "Φ"},
		{"5", "{5}", "{5}", "{1}"},
		{"6", "{6}", "{6}", "{Y}"},
		{"Y", "Φ", "Φ", "Φ"}}; // "Φ" indicates that the status is empty

	int row = input.size();		  // row of the matrix
	int column = input[0].size(); // column of the matrix

	vector<vector<string>> output;
	vector<string> header;
	int empty_index = -1;
	for (int i = 0; i < column; i++)
	{
		if (input[0][i] == "ε")
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
		cur_row.push_back(cur_status);

		// update indexes vector
		vector<string> split_stat = split(cur_status, ',');
		for (auto s : split_stat)
			indexes.push_back(state_to_index(input, s));

		for (int i = 1; i < column; i++)
		{
			// Remove the empty set
			if (input[0][i] == "ε")
				continue;
			string next_status = "";
			for (auto index : indexes)
			{
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

	cout << "output : " << endl;
	for (auto s : output)
	{
		for (auto st : s)
			cout << st << "  ";
		cout << endl;
	}

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
			row.push_back(value);
		}
		DFA.push_back(row);
	}

	// Output the result to a txt file, and use python to draw a picture according to the result
	ofstream outfile;
	outfile.open("DFA.txt");
	cout << endl
		 << "DFA : " << endl;
	for(auto h:header)
	{
		cout << h << "  ";
		outfile << h << "  ";
	}
	cout << endl;
	outfile << endl;
	for (auto s : DFA)
	{
		for (auto st : s)
		{
			cout << st << "  ";
			outfile << st << "  ";
		}
		cout << endl;
		outfile << endl;
	}

	// final state
	cout << endl
		 << "Final state : " << endl;
	for (int i = 1; i < output.size();i++)
	{
		if(output[i][0].find("Y") != string::npos)
		{
			outfile << map[output[i][0]];
			cout << map[output[i][0]];
		}
	}
	outfile << endl;
	cout << endl;
	return 0;
}

