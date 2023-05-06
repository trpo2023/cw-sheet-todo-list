all: main
main: main.cpp
	g++ -Wall -c main.cpp
	g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system