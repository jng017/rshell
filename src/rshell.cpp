#include <iostream>
#include <vector>
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

void exit(int a)
{
	return;
}

void readcommand(char *firstparse[])
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
		}
		exit(1);
	}
	else if(pid > 0)
	{
		if(-1 == wait(0))
			perror("There was an error with wait. ");
		exit(1);
	}
}

void readcommandand(char *firstparse[], char *secondparse[])
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
			perror("And case failed. Second parse will not run.");
		}
		else
		{
			if(-1 == execvp(secondparse[0], secondparse))
				perror("Second and case failed.");
		}
		exit(1);
	}
	else if(pid > 1)
	{
		if(-1 == wait(0))
			perror("There was an error using wait.");
			exit(1);
	}
}

void commandprompt()
{
	cout << "$ ";
	
	char userinput[50000];
	cin.getline(userinput, 50000, '\n');
	char *token;
	token = strtok(userinput, " ");
	
	int i = 0;
	bool noconnectors = true;
	char *parsedline[50];
	while(token != NULL)
	{
		string s = token;
		if(token[0] == '#')
		{
			break;
		}
		//else if(token[0] == '&' && token[1] == '&')
		//{
		//	noconnectors = false;
		//	char* andsituation[50];
	//		int j = 0;
	//		while(token != NULL)
	//		{
	//			token = strtok(NULL, " " );
	//			andsituation[j] = token;
	//			j++;
	//		}
	//		readcommandand(parsedline, andsituation);
	//		break;
	//	}
		//else if(token[0] == '|' && token[1] == '|')
		//{

		//}
	//	else if(s[s.size()-1] == ';')
	//	{
	//		
	//	}
//		else
//		{
			parsedline[i] = token;
			token = strtok(NULL, " ");
			i++;
//		}
	}
	if(noconnectors)
	{
		readcommand(parsedline);
	}
}


int main(int argc, char** argv)
{
	int count = 0;
	while(count < 3)
	{
		
		commandprompt();
		count++;
	}
	cout << "Exited shell. Good day." << endl;
	return 0;
}
