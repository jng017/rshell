#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

bool aflag = false;
bool lflag = false;
bool rflag = false;

void  parseinput(vector<string> &directories)
{
	//Accepts multiple directories for output. Accepts multiple components to flag.
	string getin = "";
	getline(cin, getin);
	char *cstr = new char[getin.length() + 1];
	strcpy(cstr, getin.c_str());
	char *token;
	token = strtok(cstr, " ");
	while(token!= NULL)
	{
		if(token[0] == '-')
		{
			if(token[1] == 'a')
			{
				aflag = true;
			}
			if(token[1] == 'l')
			{
				lflag = true;
			}
			if(token[1] == 'r')
			{
				rflag = true;
			}
		}
		else
		{
			//cout << token << " " << endl;
			directories.push_back(token);
		}
		token = strtok(NULL, " ");
	}
	delete [] cstr;
}

void read_directory(vector<string> &directories)
{
	char *dirName = new char[100];
	for(unsigned i = 0; i < directories.size(); i++)
	{
		strcpy(dirName, directories[i].c_str());
		DIR *dirp = opendir(dirName);
		if(dirp == NULL)
		{
			perror("Error with opendir.");
		}
		else
		{
			dirent *direntp;
			while((direntp = readdir(dirp)))
			{
				cout << direntp->d_name << endl;
			}
			closedir(dirp);
		}
		
	}
	delete [] dirName;
}

int main()
{
	//Construct char array to work with parsing options, then parse the line, opening the
	//directory as well as applying any modifiers to values to set certain parameters
	//to the function for it to work.
	vector<string> directories;
	parseinput(directories);
	read_directory(directories);
}
