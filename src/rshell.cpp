#include <iostream>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


using namespace std;

//To Do List:
//Create Connectors && || and ; and provide functionality for them.
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
	while(commandprompt())
	{	
		commandprompt();
	}
	cout << "Exited shell. Good day." << endl;
	return 0;
}
