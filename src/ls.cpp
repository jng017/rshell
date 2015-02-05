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
				cout << "-a component activated." << endl;
			}
			if(token[1] == 'l' || token[2] == 'l')
			{
				lflag = true;
				cout << "-l component activated." << endl;
			}
			if(token[1] == 'R' || token[2] == 'R')
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


void list_long(dirent *direntp)
{
	
}

//Performs directory listing of all sub-directories from within destination. Place in function
//seems to be fine; it still needs to look for the directories in the current directory
//and then the recursive function would run the same function inside those subdirectories?
void list_recursive(dirent *direntp)
{
	
}

void list_basic(dirent *direntp)
{
	if(aflag)
	{		
		cout << direntp->d_name << endl;
	}
	else
	{
		if(direntp->d_name[0] != '.')
		{
			cout << direntp->d_name << endl;
		}	
	}
}

void list_entries(vector<string> &directories)
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
			int errcheck;
			while((direntp = readdir(dirp)))
			{
				errcheck = errno;
				if(errcheck == -1)
				{
					perror("Error with readdir.");
					break;
				}
				else
				{
					if(lflag)
					{
						list_long(direntp);
					}
					else if(rflag)
					{
						list_recursive(direntp);
					}
					else
					{
						list_basic(direntp);
					}
				}
			}
			if(-1 == closedir(dirp))
			{
				perror("error with closedir.");
			}
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
	list_entries(directories);
}
