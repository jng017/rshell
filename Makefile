all: | bin
	g++ -Wall -Werror -ansi -pedantic -o bin/rshell src/main.cpp

ls: | bin
	g++ -Wall -Werror -ansi -pedantic -o bin/ls src/ls.cpp

rshell: | bin
	g++ -ansi -pedantic -Wall -Werror -o bin/rshell src/main.cpp

bin:
	mkdir bin

clean:
	rm -rf bin
