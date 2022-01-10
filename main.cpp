#include <string>
#include <iostream>
#include <fstream>
#include <io.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

int to_NFA(string path);
int NFA_to_DFA();
int minimize_DFA();
int DFA_to_code();

int get_old_file_time(string path)
{
	FILE *fp;
	int fd;
	struct stat buf;
	fp = fopen(path.c_str(), "r");
	if (NULL != fp)
	{
		fd = fileno(fp);
		fstat(fd, &buf);
		int size = buf.st_size;			 // get file size (byte)
		long modify_time = buf.st_mtime; // latest modification time (seconds passed from 01/01/00:00:00 1970 UTC)
		fclose(fp);
		return modify_time;
	}
	printf("function error\n");
	return 0;
}
int main()
{
	int fTime = 0;
	while (1)
	{
		string path = "./file/input.txt";
		// ifstream file;
		// file.open(path, ios::in);
		while (_access(path.c_str(), 0) == -1)
		{
			cout << "waiting to input" << endl;
			sleep(1);
		}
		// while (fTime == get_old_file_time(path))
		// 	sleep(1);
		// fTime = get_old_file_time(path);
		// string path1 = "./file/NFA.txt";
		// // ifstream file;
		// // file.open(path, ios::in);
		// if (_access(path1.c_str(), 0) != -1)
		// 	remove(path1.c_str());
		to_NFA(path);
		NFA_to_DFA();
		minimize_DFA();
		DFA_to_code();
		// remove(path.c_str());
	}
	return 0;
}