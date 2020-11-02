.PHONY: all
all:
	g++ -std=c++11 -g -Wall -I ./include/ ./source/main.cpp ./source/lexer.cpp ./source/tests.cpp -o a.out
tests:
	g++ -std=c++11 -g -Wall -I ./include/ ./source/lexer.cpp ./test_files/tests_e2e.cpp -o tests
.PHONY: clean
clean:
	rm tests

