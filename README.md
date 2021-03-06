#rshell
**rshell** is a budding executable file that has functionality like a shell on UNIX-operating systems, like the GNOME terminal.
This project is currently in development, and as is only a very simplistic base of a terminal. Additional functions will be added in the near future.

#How to Use
The rshell executable currently supports these capabilities of a shell.
```
Printing via command prompt.
Performing executables with argumentList functionality using execv().
Connectors(&&, ||, ;) can be used freely within the code to execute multiple commands.
Displays various error messages of executables that fail to execute.
Handle general redirection around the shell(<, >, >>, |).
Built-in `cd` command.
Exit the shell by typing 'exit'. Sending a ^C signal(SIGINT) will not interrupt the shell, but any child processes set up by the shell.
```

#Installation Guide
Follow these steps on a shell of your own to open rshell:
```
$ git clone  http://github.com/jng017/rshell.git
$ cd rshell
$ git checkout hw3
$ make
$ bin/rshell
```

#Bugs/Limitations/Issues
As this is still a project in development, this list will be updated with bugs, issues, and concerns that plague this project from completion.
```
*Random arguments may appear in the code as arguments to later executables after an executable fails, ruining the shell and requiring a restart to the shell.
*Piping(|) is not fully implemented yet.
```
