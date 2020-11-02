.PHONY: all
all:
	g++ -std=c++11 -g -Wall -I ./include/ ./source/main.cpp ./source/lexer.cpp ./source/tests.cpp -o a.out

.PHONY: clean
clean:
	rm tests

tests:
	g++ -std=c++11 -g -Wall -I ./include/ ./source/lexer.cpp ./source/parser.cpp ./test_files/tests_e2e.cpp -o tests

parser:
	g++ -std=c++11 -g -Wall -I ./include/ ./source/parser.cpp ./source/lexer.cpp -o parser

