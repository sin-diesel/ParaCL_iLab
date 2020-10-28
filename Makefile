.PHONY: all
all:
	g++ -std=c++11 -g -Wall -I ./include/ ./source/main.cpp ./source/lexer.cpp ./source/tests.cpp -o a.out

