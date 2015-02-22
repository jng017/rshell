#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


using namespace std;

bool exitbool = false;

int exit(int a)
{
	if(a == -1)
	{
		return -1;
	}
	else
	{
		return 0;
	}

}

int readcommand(char *firstparse[])
{
	int pid = fork();
	if(pid == -1)
	{
		perror("Fork error.");
		exit(1);
	}
	else if(pid == 0)
	{
		if(-1 == execvp(firstparse[0], firstparse))
		{
			perror("There was an error running execvp.");
			return exit(-1);
		}
		else
		{
			return exit(0);
		}
	}
	else if(pid > 0)
	{
		if(-1 == wait(0))
			perror("There was an error with wait. ");
		return exit(0);
	}
	return exit(0);
}

void parseinput(vector<string> &commandinput, vector<unsigned> &linemarker)
{
	string userinput;
	getline(cin, userinput);
	int u = userinput.size();
	char *cstr = new char[u];
	strcpy(cstr, userinput.c_str());
	char *token, *save_1, *token2, *save_2;
	unsigned linecount = 0;
	token = strtok_r(cstr, ";", &save_1);
	while(token != NULL)
	{
		linecount = 0;
		token2 = strtok_r(token, " ", &save_2);
		while(token2 != NULL)
		{
			commandinput.push_back(token2);
			linecount++;
			token2 = strtok_r(NULL, " ", &save_2);
		}
		linemarker.push_back(linecount);
		token = strtok_r(NULL, ";", &save_1);
	}
	delete []cstr;
}


bool process_command(char *readcommands[])
{
	int pid = fork();
	if(pid == -1)
	{
		perror("Fork error.");
		exit(1);
	}
	else if(pid == 0)
	{
		if(-1 == execvp(readcommands[0], readcommands))
		{
			perror("There was an error running execvp.");
			return false;
		}
		else
		{
			return true;
		}
	}
	else if(pid > 0)
	{
		if(-1 == wait(0))
		{
			perror("There was an error with wait. ");
			return false;
		}
	}
	return true;

}


void andor_case(vector<string> tempvect, int andpos, char* readcommands[], bool andcase, bool orcase)
{
	bool commentflag = false;
	char *andorreadcommands[1000];
	int temp = 0;
	for(unsigned i = andpos; i < tempvect.size(); i++)
	{
		for(unsigned j = 0; j < tempvect[i].size(); j++)
		{
			if(tempvect[i][j] == '#')
			{
				commentflag = true;
				break;
			}
		}
		if(commentflag)
		{
			break;
		}
		if(tempvect[i] == "&&")
		{
			andor_case(tempvect, i+1, andorreadcommands, true, false);
			break;
		}
		if(tempvect[i] == "||")
		{
			andor_case(tempvect, i+1, andorreadcommands, false, true);
			break;
		}
		else
		{
			andorreadcommands[temp] = const_cast<char*>(tempvect[i].c_str());
			temp++;
		}
	}
	if(andcase)
	{
		if(process_command(readcommands))
		{
			process_command(andorreadcommands);
		}
	}
	if(orcase)
	{
		if(!process_command(readcommands) && !process_command(andorreadcommands))
		{

		}
	}
}


void pre_process(vector<string> tempvect)
{
	bool commentflag = false;
	bool noconnectorflag = false;

	char *readcommands[1000];
	for(unsigned i = 0; i < tempvect.size(); i++)
	{
		for(unsigned j = 0; j < tempvect[i].size(); j++)
		{
			if(tempvect[i][j] == '#')
			{
				commentflag = true;
			}
		}
		if(commentflag)
		{
			break;
		}
		if(tempvect[i] == "&&")
		{
			noconnectorflag = true;
			andor_case(tempvect, i+1, readcommands, true, false);
			break;
		}
		if(tempvect[i] == "||")
		{
			noconnectorflag = true;
			andor_case(tempvect, i+1, readcommands, false, true);
			break;
		}
		else
		{
			readcommands[i] = const_cast<char*>(tempvect[i].c_str());
		}
	}
	if(!noconnectorflag)
	{
		process_command(readcommands);
	}
}

void processinput(vector<string> &commandinput, vector<unsigned> linemarker)
{
	int argcount = 0;
	int temp = 0;	
	char *cstr = new char[1000];
	for(unsigned i = 0; i < linemarker.size(); i++)
	{
		vector<string> tempvect;
		temp = 0;
		cout << "Current line: " << linemarker[i] << ": ";
		for(unsigned j = argcount; j < (linemarker[i] + argcount); j++)
		{
			cout << commandinput[j] << " ";
			tempvect.push_back(commandinput[j]);
			temp++;
		}
		argcount += temp;
		pre_process(tempvect);
		cout << endl;
	}
	delete [] cstr;
}

bool commandprompt()
{
	cout << "$ ";
	
	char userinput[50000];
	cin.getline(userinput, 50000, '\n');
	char *token, *save_1, *token2, *save_2;
	char *tokenand1, *tokenand2, *save_and_1, *save_and_2, *save_and_3;
	char *tokenor1, *tokenor2, *save_or_1, *save_or_2, *save_or_3;
	bool nospaces = true;
	token = strtok_r(userinput, ";", &save_1);
	
	char *parsedline[50];
	int i, k, andcounter, andcounter2, orcounter, orcounter2 = 0;
	bool noextras = true;
	bool andcase = false;
	bool exitshell = false;
	bool orcase = false;
	while(token != NULL)
	{
		k = i;
		nospaces = true;
		andcase = false;
		orcase = false;	
		for(int j = 0; token[j] != '\0'; ++j)
		{
			if(token[j] == 'e' && token[j+1] == 'x' && token[j+2] == 'i' && token[j+3] == 't')
			{
				exitshell = true;
			}
			if(token[j] == '#')
			{
				noextras = false;
				break;
			}
			else if(token[j] == '&' && token[j+1] == '&')
			{
				noextras = false;
				andcase = true;
			}
			else if(token[j] == '|' && token[j+1] == '|')
			{
				noextras = false;
				orcase = true;
			}
			else
			{
				continue;
			}
		}
		if(exitshell)
		{
			break;
		}
		if(andcase)
		{
			tokenand1 = strtok_r(token, "&", &save_and_1);
			tokenand2 = strtok_r(tokenand1, " ", &save_and_2);
			while(tokenand2 != NULL)
			{
				nospaces = false;
				parsedline[i] = tokenand2;
				tokenand2 = strtok_r(NULL, " ", &save_and_2);
				i++;
				andcounter++;
			}	
			if(nospaces)
			{
				parsedline[i] = tokenand2;
				i++;
				andcounter++;
			}
			char* firstparse_and[50];
			for(int a = k; a < (k+andcounter); ++a)
			{
				firstparse_and[a-k] = parsedline[a];
			}
			if(orcase)
			{
				tokenand1 = strtok_r(NULL, "&", &save_and_1);
				tokenand1 = strtok_r(NULL, "|", &save_and_1);
			}
			if(!orcase)
			{
				tokenand1 = strtok_r(NULL, "&", &save_and_1);
			}

			tokenand2 = strtok_r(tokenand1, " ", &save_and_3);
			nospaces = true;
			while(tokenand2 != NULL)
			{
				nospaces = false;
				parsedline[i] = tokenand2;
				tokenand2 = strtok_r(NULL, " ", &save_and_3);
				i++;
				andcounter2++;
			}
			if(nospaces)
			{
				parsedline[i] = tokenand2;
				i++;
				andcounter2++;
			}
			char* secondparse_and[50];
			for(int b = (k+andcounter) ; b < (k+andcounter+andcounter2); ++b)
			{
				secondparse_and[b-(k+andcounter)] = parsedline[b];
			}

			if(-1 == readcommand(firstparse_and))
			{
				break;
			}
			else
			{
				readcommand(secondparse_and);
			}

		}

		if(orcase)
		{
			tokenor1 = strtok_r(token, "|", &save_or_1);
			tokenor2 = strtok_r(tokenor1, " ", &save_or_2);
			while(tokenor2 != NULL)
			{
				nospaces = false;
				parsedline[i] = tokenor2;
				tokenor2 = strtok_r(NULL, " ", &save_or_2);
				i++;
				orcounter++;
			}	
			if(nospaces)
			{
				parsedline[i] = tokenor2;
				i++;
				orcounter++;
			}
			char* firstparse_or[50];
			for(int f = k; f < (k+orcounter); ++f)
			{
				firstparse_or[f-k] = parsedline[f];
			}
			if(andcase)
			{
				tokenor1 = strtok_r(NULL, "|", &save_or_1);
				tokenor1 = strtok_r(NULL, "&", &save_or_1);
			}
			if(!andcase)
			{
				tokenor1 = strtok_r(NULL, "|", &save_or_1);
			}

			tokenor2 = strtok_r(tokenor1, " ", &save_or_3);
			nospaces = true;
			while(tokenor2 != NULL)
			{
				nospaces = false;
				parsedline[i] = tokenor2;
				tokenor2 = strtok_r(NULL, " ", &save_or_3);
				i++;
				orcounter2++;
			}
			if(nospaces)
			{
				parsedline[i] = tokenor2;
				i++;
				orcounter2++;
			}
			char* secondparse_or[50];
			for(int p = (k+orcounter) ; p < (k+orcounter+orcounter2); ++p)
			{
				secondparse_or[p-(k+orcounter)] = parsedline[p];
			}

			if(-1 != readcommand(firstparse_or))
			{
				break;
			}
			readcommand(secondparse_or);
		}
		else
		{
			token2 = strtok_r(token, " ", &save_2);
			while(token2 != NULL)
			{
				nospaces = false;
				parsedline[i] = token2;
				token2 = strtok_r(NULL, " ", &save_2);
				i++;
			}
			if(nospaces)
			{
				parsedline[i] = token2;
				i++;
			}
			if(noextras)
			{
				char* parsedlinecopy[50];
				for(int a = k; a <= i; ++a)
				{
					parsedlinecopy[a-k] = parsedline[a];
				}
				readcommand(parsedlinecopy);
			}
			token = strtok_r(NULL, ";", &save_1);
		}
	}
	if(exitshell)
	{
		return false;
	}
	else
	{
		return true;
	}
}


int main(int argc, char** argv)
{
	//Place while loop here.
	vector<unsigned> linemarker;
	vector<string> commandinput;
	cout << "$ ";
	parseinput(commandinput, linemarker);
	processinput(commandinput, linemarker);
	//for(unsigned i = 0; i < commandinput.size(); i++)
	//{
	//	cout << commandinput[i] << " ";
	//}
	cout << "Exited shell. Good day." << endl;
	return 0;
}
