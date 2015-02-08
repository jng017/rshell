#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
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
				cout << "-R component activated." << endl;
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
	struct stat info;
	if(-1 == stat(direntp->d_name, &info))	
	{
		perror("Error with stat");
		exit(EXIT_SUCCESS);
	}
	mode_t permissions = info.st_mode;

	cout << S_ISREG(permissions) << S_ISDIR(permissions);


	cout << direntp->d_name << endl;
}

//Performs directory listing of all sub-directories from within destination. Place in function
//seems to be fine; it still needs to look for the directories in the current directory
//and then the recursive function would run the same function inside those subdirectories?
void list_recursive(dirent *direntp)
{
}

void sort_basic(dirent *direntp, vector<string> &files)
{
	if(aflag)
	{		
		files.push_back(direntp->d_name);
	}
	else
	{
		if(direntp->d_name[0] != '.')
		{
			files.push_back(direntp->d_name);
		}	
	}
}

void list_entries(vector<string> &directories, vector<string> &files)
{
	char *dirName = new char[100];
	for(unsigned i = 0; i < directories.size(); i++)
	{
		strcpy(dirName, directories[i].c_str());
		DIR *dirp = opendir(dirName);
		if(dirp == NULL)
		{
			perror("Error with opendir.");
			exit(EXIT_SUCCESS);
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
				if(rflag)
				{
					list_recursive(direntp);	
				}
				else
				{
					sort_basic(direntp, files);
				}
			}
			if(-1 == closedir(dirp))
			{
				perror("error with closedir.");
				exit(EXIT_SUCCESS);
			}
		}
		
	}
	delete [] dirName;
}

void sort_files(vector<string> &files)
{
	string x;
	for(unsigned i = 0; i < files.size(); i++)
	{
		unsigned j = i;
		while(j > 0 && files[j-1] > files[j])
		{
			swap(files[j], files[j-1]);
			j = j -1;
		}
	}
}

int main()
{
	//Construct char array to work with parsing options, then parse the line, opening the
	//directory as well as applying any modifiers to values to set certain parameters
	//to the function for it to work.
	vector<string> directories;
	vector<string> files;
	parseinput(directories);
	list_entries(directories, files);
	for(unsigned i = 0; i < files.size(); i++)
	{
		cout << files[i] << " ";
	}
	cout << endl << "Sorted Files" << endl;
	sort_files(files);
	for(unsigned j = 0; j < files.size(); j++)
	{
		cout << files[j] << " ";
	}
	cout << endl;
}
