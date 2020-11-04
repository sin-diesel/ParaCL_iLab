CXX = g++
IDIR = ./include
CXXFLAGS = -g -Wall -std=c++11 -I $(IDIR)
SRC_DIR = source
OBJ_DIR = obj
SOURCES = lexer.cpp main.cpp tests.cpp parser.cpp
HEADERS = lexer.h tests.h parser.h
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all
all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o compiler
	# mv $(OBJECTS) $(OBJ_DIR)

lexer.o:
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/lexer.cpp -c -o lexer.o

main.o:
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/main.cpp -c -o main.o

tests.o:
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/tests.cpp -c -o tests.o

parser.o:
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/parser.cpp -c -o parser.o

.PHONY: clean
clean:
	rm $(OBJECTS)