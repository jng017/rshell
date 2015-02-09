all: | bin
	g++ -Wall -Werror -ansi -pedantic -o bin/rshell src/main.cpp
	g++ -Wall -Werror -ansi -pedantic -o bin/ls src/ls.cpp
	g++ -Wall -Werror -ansi -pedantic -o bin/cp src/cp.cpp

ls: | bin
	g++ -Wall -Werror -ansi -pedantic -o bin/ls src/ls.cpp

rshell: | bin
	g++ -ansi -pedantic -Wall -Werror -o bin/rshell src/main.cpp

cp: | bin
	g++ -Wall -Werror -ansi -pedantic -o bin/cp src/cp.cpp

bin:
	mkdir bin

clean:
	rm -rf bin
