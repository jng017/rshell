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
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <iomanip>
#include <sstream>

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

void list_long(vector<string> &files)
{
	blksize_t total_blocks;
	for(unsigned i = 0; i < files.size(); i++)
	{
		const char* c;
		c = files[i].c_str();
		struct stat info;
		if(-1 == stat(c, &info))	
		{
			perror("Error with stat");
			exit(EXIT_SUCCESS);
		}
		total_blocks += info.st_blksize;
		string permstring = "";
		mode_t perm = info.st_mode;
		if(S_ISDIR(perm) == 1)
		{
			permstring.push_back('d');
		}
		if(S_ISLNK(perm) == 1)
		{
			permstring.push_back('l');
		}
		if(S_ISREG(perm) == 1)
		{
			permstring.push_back('-');
		}
		if(perm & S_IRUSR) //User permissions
		{
			permstring.push_back('r');
		}
		if(!(perm & S_IRUSR))
		{
			permstring.push_back('-');
		}
		if(perm & S_IWUSR)
		{
			permstring.push_back('w');
		}
		if(!(perm & S_IWUSR))
		{
			permstring.push_back('-');
		}
		if(perm & S_IXUSR)
		{
			permstring.push_back('x');
		}
		if(!(perm & S_IXUSR))
		{
			permstring.push_back('-');
		}
		if(perm & S_IRGRP) //Group permissions
		{
			permstring.push_back('r');
		}
		if(!(perm & S_IRGRP))
		{
			permstring.push_back('-');
		}
		if(perm & S_IWGRP)
		{
			permstring.push_back('w');
		}
		if(!(perm & S_IWGRP))
		{
			permstring.push_back('-');
		}
		if(perm & S_IXGRP)
		{
			permstring.push_back('x');
		}
		if(!(perm & S_IXGRP))
		{
			permstring.push_back('-');
		}
		if(perm & S_IROTH) //Others permissions
		{
			permstring.push_back('r');
		}
		if(!(perm & S_IROTH))
		{
			permstring.push_back('-');
		}
		if(perm & S_IWOTH)
		{
			permstring.push_back('w');
		}
		if(!(perm & S_IWOTH))
		{
			permstring.push_back('-');
		}
		if(perm & S_IXOTH)
		{
			permstring.push_back('x');
		}
		if(!(perm & S_IXOTH))
		{
			permstring.push_back('-');
		}

		struct passwd* userinfo;
		userinfo = getpwuid(info.st_uid);

		struct group* groupinfo;
		groupinfo = getgrgid(info.st_gid);

		//cout << ctime(&info.st_mtime);
		struct tm* timeinfo;
		timeinfo = localtime(&(info.st_mtime));
		string mon = "";
		string day = "";
		string hourmin = "";
		if(timeinfo->tm_mon == 0)
		{
			mon = "Jan";
		}
		if(timeinfo->tm_mon == 1)
		{
			mon = "Feb";
		}
		if(timeinfo->tm_mon == 2)
		{
			mon = "Mar";
		}
		if(timeinfo->tm_mon == 3)
		{
			mon = "Apr";
		}
		if(timeinfo->tm_mon == 4)
		{
			mon = "May";
		}
		if(timeinfo->tm_mon == 5)
		{
			mon = "Jun";
		}
		if(timeinfo->tm_mon == 6)
		{
			mon = "Jul";
		}
		if(timeinfo->tm_mon == 7)
		{
			mon = "Aug";
		}
		if(timeinfo->tm_mon == 8)
		{
			mon = "Sep";
		}
		if(timeinfo->tm_mon == 9)
		{
			mon = "Oct";
		}
		if(timeinfo->tm_mon == 10)
		{
			mon = "Nov";
		}
		if(timeinfo->tm_mon == 11)
		{
			mon = "Dec";
		}

		if(timeinfo->tm_mday == 1)
		{
			day = "1";
		}
		if(timeinfo->tm_mday == 2)
		{
			day = "2";
		}
		if(timeinfo->tm_mday == 3)
		{
			day = "3";
		}
		if(timeinfo->tm_mday == 4)
		{
			day = "4";
		}
		if(timeinfo->tm_mday == 5)
		{
			day = "5";
		}
		if(timeinfo->tm_mday == 6)
		{
			day = "6";
		}
		if(timeinfo->tm_mday == 7)
		{
			day = "7";
		}
		if(timeinfo->tm_mday == 8)
		{
			day = "8";
		}
		if(timeinfo->tm_mday == 9)
		{
			day = "9";
		}
		if(timeinfo->tm_mday == 10)
		{
			day = "10";
		}
		if(timeinfo->tm_mday == 11)
		{
			day = "11";
		}
		if(timeinfo->tm_mday == 12)
		{
			day = "12";
		}
		if(timeinfo->tm_mday == 13)
		{
			day = "13";
		}
		if(timeinfo->tm_mday == 14)
		{
			day = "14";
		}
		if(timeinfo->tm_mday == 15)
		{
			day = "15";
		}
		if(timeinfo->tm_mday == 16)
		{
			day = "16";
		}
		if(timeinfo->tm_mday == 17)
		{
			day = "17";
		}
		if(timeinfo->tm_mday == 18)
		{
			day = "18";
		}
		if(timeinfo->tm_mday == 19)
		{
			day = "19";
		}
		if(timeinfo->tm_mday == 20)
		{
			day = "20";
		}
		if(timeinfo->tm_mday == 21)
		{
			day = "21";
		}
		if(timeinfo->tm_mday == 22)
		{
			day = "22";
		}
		if(timeinfo->tm_mday == 23)
		{
			day = "23";
		}
		if(timeinfo->tm_mday == 24)
		{
			day = "24";
		}
		if(timeinfo->tm_mday == 25)
		{
			day = "25";
		}
		if(timeinfo->tm_mday == 26)
		{
			day = "26";
		}
		if(timeinfo->tm_mday == 27)
		{
			day = "27";
		}
		if(timeinfo->tm_mday == 28)
		{
			day = "28";
		}
		if(timeinfo->tm_mday == 29)
		{
			day = "29";
		}
		if(timeinfo->tm_mday == 30)
		{
			day = "30";
		}
		if(timeinfo->tm_mday == 31)
		{
			day = "31";
		}

		if(timeinfo->tm_hour == 0)
		{
			hourmin += "00:";
		}
		if(timeinfo->tm_hour == 1)
		{
			hourmin += "01:";
		}
		if(timeinfo->tm_hour == 2)
		{
			hourmin += "02:";
		}
		if(timeinfo->tm_hour == 3)
		{
			hourmin += "03:";
		}
		if(timeinfo->tm_hour == 4)
		{
			hourmin += "04:";
		}
		if(timeinfo->tm_hour == 5)
		{
			hourmin += "05:";
		}
		if(timeinfo->tm_hour == 6)
		{
			hourmin += "06:";
		}
		if(timeinfo->tm_hour == 7)
		{
			hourmin += "07:";
		}
		if(timeinfo->tm_hour == 8)
		{
			hourmin += "08:";
		}
		if(timeinfo->tm_hour == 9)
		{
			hourmin += "09:";
		}
		if(timeinfo->tm_hour == 10)
		{
			hourmin += "10:";
		}
		if(timeinfo->tm_hour == 11)
		{
			hourmin += "11:";
		}
		if(timeinfo->tm_hour == 12)
		{
			hourmin += "12:";
		}
		if(timeinfo->tm_hour == 13)
		{
			hourmin += "13:";
		}
		if(timeinfo->tm_hour == 14)
		{
			hourmin += "14:";
		}
		if(timeinfo->tm_hour == 15)
		{
			hourmin += "15:";
		}
		if(timeinfo->tm_hour == 16)
		{
			hourmin += "16:";
		}
		if(timeinfo->tm_hour == 17)
		{
			hourmin += "17:";
		}
		if(timeinfo->tm_hour == 18)
		{
			hourmin += "18:";
		}
		if(timeinfo->tm_hour == 19)
		{
			hourmin += "19:";
		}
		if(timeinfo->tm_hour == 20)
		{
			hourmin += "20:";
		}
		if(timeinfo->tm_hour == 21)
		{
			hourmin += "21:";
		}
		if(timeinfo->tm_hour == 22)
		{
			hourmin += "22:";
		}
		if(timeinfo->tm_hour == 23)
		{
			hourmin += "23:";
		}

		if(timeinfo->tm_min == 0)
		{
			hourmin += "00";
		}
		if(timeinfo->tm_min == 1)
		{
			hourmin += "01";
		}
		if(timeinfo->tm_min == 2)
		{
			hourmin += "02";
		}
		if(timeinfo->tm_min == 3)
		{
			hourmin += "03";
		}
		if(timeinfo->tm_min == 4)
		{
			hourmin += "04";
		}
		if(timeinfo->tm_min == 5)
		{
			hourmin += "05";
		}
		if(timeinfo->tm_min == 6)
		{
			hourmin += "06";
		}
		if(timeinfo->tm_min == 7)
		{
			hourmin += "07";
		}
		if(timeinfo->tm_min == 8)
		{
			hourmin += "08";
		}
		if(timeinfo->tm_min == 9)
		{
			hourmin += "09";
		}
		if(timeinfo->tm_min == 10)
		{
			hourmin += "10";
		}
		if(timeinfo->tm_min == 11)
		{
			hourmin += "11";
		}
		if(timeinfo->tm_min == 12)
		{
			hourmin += "12";
		}
		if(timeinfo->tm_min == 13)
		{
			hourmin += "13";
		}
		if(timeinfo->tm_min == 14)
		{
			hourmin += "14";
		}
		if(timeinfo->tm_min == 15)
		{
			hourmin += "15";
		}
		if(timeinfo->tm_min == 16)
		{
			hourmin += "16";
		}
		if(timeinfo->tm_min == 17)
		{
			hourmin += "17";
		}
		if(timeinfo->tm_min == 18)
		{
			hourmin += "18";
		}
		if(timeinfo->tm_min == 19)
		{
			hourmin += "19";
		}
		if(timeinfo->tm_min == 20)
		{
			hourmin += "20";
		}
		if(timeinfo->tm_min == 21)
		{
			hourmin += "21";
		}
		if(timeinfo->tm_min == 22)
		{
			hourmin += "22";
		}
		if(timeinfo->tm_min == 23)
		{
			hourmin += "23";
		}
		if(timeinfo->tm_min == 24)
		{
			hourmin += "24";
		}
		if(timeinfo->tm_min == 25)
		{
			hourmin += "25";
		}
		if(timeinfo->tm_min == 26)
		{
			hourmin += "26";
		}
		if(timeinfo->tm_min == 27)
		{
			hourmin += "27";
		}
		if(timeinfo->tm_min == 28)
		{
			hourmin += "28";
		}
		if(timeinfo->tm_min == 29)
		{
			hourmin += "29";
		}
		if(timeinfo->tm_min == 30)
		{
			hourmin += "30";
		}
		if(timeinfo->tm_min == 31)
		{
			hourmin += "31";
		}
		if(timeinfo->tm_min == 32)
		{
			hourmin += "32";
		}
		if(timeinfo->tm_min == 33)
		{
			hourmin += "33";
		}
		if(timeinfo->tm_min == 34)
		{
			hourmin += "34";
		}
		if(timeinfo->tm_min == 35)
		{
			hourmin += "35";
		}
		if(timeinfo->tm_min == 36)
		{
			hourmin += "36";
		}
		if(timeinfo->tm_min == 37)
		{
			hourmin += "37";
		}
		if(timeinfo->tm_min == 38)
		{
			hourmin += "38";
		}
		if(timeinfo->tm_min == 39)
		{
			hourmin += "39";
		}
		if(timeinfo->tm_min == 40)
		{
			hourmin += "40";
		}
		if(timeinfo->tm_min == 41)
		{
			hourmin += "41";
		}
		if(timeinfo->tm_min == 42)
		{
			hourmin += "42";
		}
		if(timeinfo->tm_min == 43)
		{
			hourmin += "43";
		}
		if(timeinfo->tm_min == 44)
		{
			hourmin += "44";
		}
		if(timeinfo->tm_min == 45)
		{
			hourmin += "45";
		}
		if(timeinfo->tm_min == 46)
		{
			hourmin += "46";
		}
		if(timeinfo->tm_min == 47)
		{
			hourmin += "47";
		}
		if(timeinfo->tm_min == 48)
		{
			hourmin += "48";
		}
		if(timeinfo->tm_min == 49)
		{
			hourmin += "49";
		}
		if(timeinfo->tm_min == 50)
		{
			hourmin += "50";
		}
		if(timeinfo->tm_min == 51)
		{
			hourmin += "51";
		}
		if(timeinfo->tm_min == 52)
		{
			hourmin += "52";
		}
		if(timeinfo->tm_min == 53)
		{
			hourmin += "53";
		}
		if(timeinfo->tm_min == 54)
		{
			hourmin += "54";
		}
		if(timeinfo->tm_min == 55)
		{
			hourmin += "55";
		}
		if(timeinfo->tm_min == 56)
		{
			hourmin += "56";
		}
		if(timeinfo->tm_min == 57)
		{
			hourmin += "57";
		}
		if(timeinfo->tm_min == 58)
		{
			hourmin += "58";
		}
		if(timeinfo->tm_min == 59)
		{
			hourmin += "59";
		}
		if(timeinfo->tm_min == 60)
		{
			hourmin += "60";
		}
	//	cout << "Test Time Values: " << mon << " " << day << " " << hourmin << " ";
		cout << left << permstring << " " << info.st_nlink << " " << userinfo->pw_name << " " << groupinfo->gr_name << " " << info.st_size << " " << mon << " " << day << " " << hourmin << " "<< files[i] << endl;
		//cout << (long) info.st_ino; For printing inode
	}
	cout << "total" << (total_blocks)/160 << endl;
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
//	for(unsigned i = 0; i < files.size(); i++)
//	{
//		cout << files[i] << " ";
//	}
//	cout << endl << "Sorted Files" << endl;
	sort_files(files);
	if(lflag)
	{
		list_long(files);
	}
	else
	{
		for(unsigned j = 0; j < files.size(); j++)
		{
			cout << files[j] << " ";
		}
		cout << endl;
	}
	
}
