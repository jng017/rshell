VPATH = src

all: rshell.cpp
	g++ -Wall -Werror -ansi -pedantic

rshell: rshell.cpp
	g++ -Wall -Werror -ansi -pedantic rshell.cpp

bin:
	mkdir bin
