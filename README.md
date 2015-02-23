#rshell
**rshell** is a budding executable file that has functionality like a shell on UNIX-operating systems, like the GNOME terminal.
This project is currently in development, and as is only a very simplistic base of a terminal. Additional functions will be added in the near future.

#How to Use
The rshell executable currently supports these capabilities of a shell.
```
Printing via command prompt.
Performing executables with argumentList functionality.
Connectors(&&, ||, ;) can be used freely within the code to execute multiple commands.
Displays various error messages of executables that fail to execute.
Handle general redirection around the shell(<, >, >>, |).
Exit the shell by typing 'exit'.
```

#Installation Guide
Follow these steps on a shell of your own to open rshell:
```
$ git clone  http://github.com/yourusername/rshell.git
$ cd rshell
$ git checkout hw2
$ make
$ bin/rshell
```

#Bugs/Limitations/Issues
As this is still a project in development, this list will be updated with bugs, issues, and concerns that plague this project from completion.
```
*If there are special cases(&&, ||, <, etc.), the shell will run into executable errors
on subsequent callings of functions. The cause of this seems to be how the shell is coded
to handle arguments after the connector/special case symbol.
*Unable to handle input redirection efficiently; only seems to work when an executable or
command line has already been parsed into the shell. Otherwise, it will hang.
*Piping is not fully implemented yet, and hangs on the 2nd child process.
```
