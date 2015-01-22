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

//To-Do List:
//Print $ on every command line for user input.
//When user input is accepted, first go through looking for a stop in ; or a space for an executable. Then, execute a function that will fork + attempt to run executable.
//If executable run is not successful, output command not found or something. if found, executable will run. End the child process either way. Don't forget perror.
//Parent child should have been on wait, then resume by continuing to ask for user-input.

void exit(int a)
{
	return;
}

void readcommand(char* cmdinput[])
{
	int pid = fork();
	if(pid == -1)
	{
		perror("Fork error.");
		exit(1);
	}
	else if(pid == 0)
	{
		//Check executable here.
		cout << "Got to child process. Checking/running executable." << endl;
		if(-1 == execvp(cmdinput[0], cmdinput))
		{
			perror("There was an error running execvp.");
		}
		//likely where commands + extensions are executed here.
		exit(1);
	}
	else if(pid > 0)
	{
		if(-1 == wait(0))
			perror("There was an error with wait. ");
		cout << "Returning to parent function." << endl;
		exit(1);
	}
}

void commandprompt()
{
	cout << "$ ";
	char terminput[10000];
	cin.getline(terminput, 10000, '\n');
	char* cmdinput[100];
	bool firstword = true;
	char *token;
	int i = 0;
	token = strtok(terminput, " ");
	while(token != NULL)
	{
		cmdinput[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	cout << "Entered input. Running fork()." << endl;

	readcommand(cmdinput);
}



int main(int argc, char** argv)
{
	vector<string> readline;

	char* parsedargv[100];
	int count = 0;
	while(count < 3)
	{
		
		commandprompt();
		count++;
	}
	cout << "Exited shell. Good day." << endl;
	return 0;
}
