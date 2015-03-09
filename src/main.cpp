#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

bool exitbool = false;

void handler(int i)
{
	if(i==SIGINT)
	{
		//cout << "Returned to shell" << endl;	
	}
	//if(i==SIGTSTP)
	//{
	//	cerr << " Not pausing rshell," << endl;
	//}
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

bool change_dir(char *dir)
{
	//cout << "Changing directory to: " << readdirectory << endl;
	if(-1 == chdir(dir))
	{
		perror("There was an error with chdir(). ");
		return false;
	}
	setenv("PWD", getcwd(NULL, 0), 1);
	return true; //Successfully changed dir.
}

vector<string> possible_dir(char *exec)
{
	char *r = getenv("PATH");
	char *t = new char[strlen(r) + 1];
	strcpy(t, r);
	vector<string> tempvctr;
	//cerr << t << endl;
	char *token, *save_1;
	token = strtok_r(t, ":", &save_1);
	while(token != NULL)
	{
		string tempstr = token;
		if(exec[0] != '/')
		{
			tempstr += "/";
		}
		tempstr += exec;
		tempvctr.push_back(tempstr);
		token = strtok_r(NULL, ":", &save_1);
	}
	//for(unsigned i = 0; i < tempvctr.size(); i++)
	//{
	//	cerr << tempvctr[i] << endl;
	//}
	delete []t;
	return tempvctr;
}

//Use execv, the first argument is the filename + its path(will have to append to this then), and then the second argument should be the arguments that follow. Maybe strcat within htis function to readcommands[0]?
bool process_command(vector<string> snippet, int type, char *ioarg, int anyfd)
{
	bool ret = true;
	char **readcommands = new char*[snippet.size()];
	for(unsigned a = 0; a < snippet.size(); a++)
	{
		readcommands[a] = const_cast<char*>(snippet[a].c_str());
	}
	vector<string> paths = possible_dir(readcommands[0]);

	int pid = fork();
	int fd = -1;
	int fdsave = -1;
//	int pipefd[2];
//	pipe(pipefd);
		if(pid <= -1)
	{
		perror("Fork error.");
		_exit(EXIT_FAILURE);
	}
	else if(pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		bool execvpassed = false;
		
		if(type == 0 || type == 1 || type == 2)
		{
			if(type == 0)
			{
				if(-1 == (fd = open(ioarg, O_CREAT|O_RDWR, S_IREAD|S_IWRITE)))
				{
					perror("There was an error with open() for <. ");
				}
			}
			if(type == 1)
			{
				if(-1 == (fd = open(ioarg, O_CREAT | O_RDWR, S_IREAD | S_IWRITE)))
				{
					perror("There was an error with open() for >. ");
				}
			}
			if(type == 2)
			{
				if(-1 == (fd = open(ioarg, O_APPEND | O_RDWR, S_IREAD | S_IWRITE)))
				{
					perror("There was an error with open() for >>. ");
				}
				type--;
			}
			fdsave = dup(type);
			if(-1 == dup2(fd, anyfd))
			{
				perror("There was an error with dup2(). ");
			}
			if(-1 == close(fd))
			{
				perror("There was an error with close(). ");
			}
		}
		

		for(unsigned i = 0; i < paths.size(); i++)
		{
			readcommands[0] = const_cast<char*>(paths[i].c_str());
			if(-1 != execv(readcommands[0], readcommands))
			{	
				execvpassed = true;
			}
		}
		if(!execvpassed)
		{
			perror("There was an error with execv(). ");
			_exit(EXIT_FAILURE);
		}

		if(type == 0 || type == 1 || type == 2)
		{
			if(-1 == dup2(fdsave, type))
			{
				perror("There was an error with dup2() on exit. ");
			}
		}
	}
	else if(pid > 0)
	{
		int status;
		if(-1 == waitpid(-1, &status, 0))
		{
			perror("There was an error with wait(). ");
			_exit(EXIT_FAILURE);
		}
		if(WEXITSTATUS(status) != 0)
		{
			//cout << "Child did not exit successfully." << endl;
			ret = false;
		}
	}
	delete[]readcommands;
	return ret;
}

void process_pipe(vector<vector<string> > pipeargs)
{
	for(unsigned i = 0; i < pipeargs.size() - 1; i+=1)
	{
		char **pipearg1= new char*[pipeargs[i].size()];
		char **pipearg2 = new char*[pipeargs[i+1].size()];
		for(unsigned a = 0; a < pipeargs[i].size(); a++)
		{
			pipearg1[a] = const_cast<char*>(pipeargs[i][a].c_str());
		}
		for(unsigned b = 0; b < pipeargs[i+1].size(); b++)
		{
			pipearg2[b] = const_cast<char*>(pipeargs[i+1][b].c_str());
		}
		vector<string> paths1 = possible_dir(pipearg1[0]);
		vector<string> paths2 = possible_dir(pipearg2[0]);
		int pipefd[2];
		pipe(pipefd);
		//cout << "hi" << endl;
		int pid = fork();
		if(pid == -1)
		{
			perror("There was an error with fork(). ");
			_exit(EXIT_FAILURE);
		}
		else if(pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGTSTP, SIG_DFL);
			bool execvpassed = false;
			close(pipefd[0]);
	
			dup2(pipefd[1], 1);
				
			close(pipefd[1]);
	
			for(unsigned i = 0; i < paths1.size(); i++)
			{
				pipearg1[0] = const_cast<char*>(paths1[i].c_str());
				if(-1 != execv(pipearg1[0], pipearg1))
				{	
					execvpassed = true;
				}
			}
			if(!execvpassed)
			{
				perror("There was an error with execv(). ");
				_exit(EXIT_FAILURE);
			}

		}
		int savestdin = -1;
		if(pid > 0)
		{
			if(-1 == (savestdin = dup(0)))
			{
				perror("There was an error with dup(). ");
			}
			if(-1 == wait(0))
			{
				perror("There was an error with wait(). ");
			}

			int pid2 = fork();
			if(pid2 == -1)
			{
				perror("There was an error with fork(). ");
				_exit(EXIT_SUCCESS);
			}
			else if(pid2 == 0)
			{

				signal(SIGINT, SIG_DFL);
				signal(SIGTSTP, SIG_DFL);
				bool execvpassed = false;
				if(-1 ==  close(pipefd[1]))
				{
					perror("There was an error with close(). ");
				}
				if(-1 == dup2(pipefd[0], 0))
				{
					perror("There was an error with dup2(). ");
				}
				if(-1 == close(pipefd[0]))
				{
					perror("There was an error with close(). ");
				}

				for(unsigned i = 0; i < paths2.size(); i++)
				{
					pipearg2[0] = const_cast<char*>(paths2[i].c_str());
					if(-1 != execv(pipearg2[0], pipearg2))
					{	
						execvpassed = true;
					}
					if(!execvpassed)
					{
						perror("There was an error with execv(). ");
						_exit(EXIT_FAILURE);
					}
				}
			}
			else if(pid2 > 0)
			{
				if(-1 == wait(0))
				{
					perror("There was an error with wait(). ");
				}
			}
		}
		if(-1 == dup2(savestdin, 0))
		{
			perror("There was an error with dup2(). ");
		}
	}	
}	

bool process_singleline(vector<string> tempvect, int index)
{
	bool commentflag = false;
	bool specialflag = false;
	bool andcase = false;
	bool orcase = false;
	//bool pipeflag = false;

	vector<string> snippet;
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
		if(tempvect[i] == "exit")
		{
			cout << "Found exit" << endl;
			exitbool = true;
			return false;
		}
		if(tempvect[i] == "&&")
		{
			//andcase = true;
			specialflag = true;
			//cout << "Working on and case for" << endl;
			//for(unsigned b = 0; b < snippet.size(); b++)
			//{
			//	cout << snippet[b] << " ";
			//}
			//cout << endl;
			if(process_command(snippet, -1, NULL, -1))
			{
				//cout << "andcase passed" << endl;
				andcase = true;
			}
			snippet.clear();
			continue;
		}
		if(tempvect[i] == "||")
		{
			//orcase = true;
			specialflag = true;
			//cout << "Working on or case for" << endl;
			//for(unsigned b = 0; b < snippet.size(); b++)
			//{
			//	cout << snippet[b] << " ";
			//}
			//cout << endl;
			if(!process_command(snippet, -1, NULL, -1))
			{
				//cout << "Or case passed" << endl;
				orcase = true;
			}
			snippet.clear();
			continue;
		}
		if(tempvect[i] == "<" && tempvect.size() > 1)
		{
			//ioinputcase = true;
			specialflag = true;
			//cout << "Entered input redirection phase." << endl;
			//for(unsigned b = 0; b < snippet.size(); b++)
			//{
			//	cout << snippet[b] << " ";
			//}
			//cout << tempvect[i+1] << endl;
			//cout << endl;
			if(process_command(snippet, 0, const_cast<char*>(tempvect[i+1].c_str()), 0))
			{
				
			}
			i += 1;
			//cout << "The string after this: " << tempvect[i];
			snippet.clear();
			continue;
		}
		if(tempvect[i] == ">" && tempvect.size() > 1)
		{
			//iooutputcase = true;
			specialflag = true;
			cout << "Entered > redirection." << endl;
			if(process_command(snippet, 1, const_cast<char*>(tempvect[i+1].c_str()), 1))
			{
			}
			i += 1;
			//cout << "The string after this: " << tempvect[i];
			snippet.clear();
			continue;
		}
		if(tempvect[i] == ">>" && tempvect.size() > 1)
		{
			//iooutputcase2 = true;
			specialflag = true;
			//cout << "Entered >> redirection." << endl;
			if(process_command(snippet, 2, const_cast<char*>(tempvect[i+1].c_str()), 2))
			{
			}
			i += 1;
			//cout << "The string after this: " << tempvect[i] << endl;
			snippet.clear();
			continue;
		}
		if(std::string::npos != tempvect[i].find('>', 1) && std::string::npos == tempvect[i].find('>', 2))
		{
			specialflag = true;
			//cout << "Entered any file descriptor > redirection." << endl;
			if(process_command(snippet, 1, const_cast<char*>(tempvect[i+1].c_str()), tempvect[i][0]))
			{
			}
			i += 1;
			//cout << "The string after this: " << tempvect[i] << endl;
			snippet.clear();
		}
		if(std::string::npos != tempvect[i].find('>', 1) && std::string::npos != tempvect[i].find('>', 2))
		{
			specialflag = true;
			//cout << "Entered any file descriptor >> redirection." << endl;
			if(process_command(snippet, 2, const_cast<char*>(tempvect[i+1].c_str()), tempvect[i][0]))
			{
			}
			i += 1;
			//cout << "The string after this: " << tempvect[i] << endl;
			snippet.clear();
		}
		if(tempvect[i] == "|")
		{
			//iopipecase = true;1
			//pipeflag = true;
			//specialflag = true;
			//continue;
		}
		if(tempvect[i] == "cd" && tempvect.size() > 1)
		{
			//cdcase=true
			specialflag = true;
			change_dir(const_cast<char*>(tempvect[i+1].c_str()));
			break;
		}
		if(tempvect[i] == "cd" && tempvect.size() == 1)
		{
			specialflag = true;
			tempvect.push_back("/home");
			change_dir(const_cast<char*>(tempvect[i+1].c_str()));
		}
		else
		{
			snippet.push_back(tempvect[i]);
		}
	}
	cout << "What's currently in snippet after running through line." << endl;
	for(unsigned c = 0; c < snippet.size(); c++)
	{
		cout << snippet[c] << " ";
	}
	//cout << endl;
	if(andcase)
	{
		process_command(snippet, -1, NULL, -1);
	}
	if(orcase)
	{
		process_command(snippet, -1, NULL, -1);
	}
	//if(pipeflag && specialflag)
	//{
	//	process_command 
	//	cout << "Entered | redirection." << endl;
	//	vector<vector<string> > pipeargs;
	//	int pipeindex = 0;
	//	for(unsigned d = 0; d < snippet.size(); d++)
	//	{
	//		if(snippet[d] == "|")
	//		{
	//			pipeindex++;
	//		}
	//		else
	//		{
	//			pipeargs[pipeindex].push_back(snippet[d]);
	//		}
	//	}
	//	process_pipe(pipeargs);
	//}
	if(!specialflag)
	{
		process_command(snippet, -1, NULL, -1);
	}
	snippet.clear();
	return true;
}

void processlines(vector<string> &commandinput, vector<unsigned> linemarker)
{
	int argcount = 0;
	int temp = 0;
	vector<string> tempvect;;
	for(unsigned i = 0; i < linemarker.size(); i++)
	{
		tempvect.clear();
		temp = 0;
		for(unsigned j = argcount; j < (linemarker[i] + argcount); j++)
		{
			if(commandinput[j] == "exit" || commandinput[j] == "Exit" || commandinput[j] == "EXIT")
			{
				exitbool = true;
				cout << "Exited Shell. Good Day." << endl;
				raise(SIGINT);
				exit(0);
				break;
			}
			else
			{
				tempvect.push_back(commandinput[j]);
				temp++;
			}
		}
		if(exitbool)
		{
			break;
		}
		else
		{
			argcount += temp;
			if(!process_singleline(tempvect, 0))
			{
				exit(0);
			}
		}
	}
}

int main(int argc, char** argv)
{
	char hostname[120];
	char *loginname = getlogin();
	vector<unsigned> linemarker;
	vector<string> commandinput;
	gethostname(hostname, sizeof(hostname));
	if(loginname == NULL)
	{
		perror("There was an error obtaining the username with getlogin(). ");
	}
	while(!exitbool)
	{
		signal(SIGINT, handler);
		//signal(SIGTSTP, handler);
		cout << loginname << "@" << hostname << ":" << getenv("PWD") << "% ";
		parseinput(commandinput, linemarker);
		processlines(commandinput, linemarker);
		linemarker.clear();
		commandinput.clear();
	}
}
