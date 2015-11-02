# nicole cranon <nicole.cranon@ucdenver.edu>
# csci 4640, fall 2015
# homework 4 - universal scanner
# makefile

CC=g++ -std=c++11
CFLAGS=-c -Wall -g
INC=-I ./include 

scanner: main.o uscanner.o 
	$(CC) main.o uscanner.o -o scanner

main.o: ./src/main.cpp
	$(CC) $(INC) $(CFLAGS) ./src/main.cpp -o main.o

uscanner.o: ./src/uscanner.cpp
	$(CC) $(INC) $(CFLAGS) ./src/uscanner.cpp -o uscanner.o

clean:
	rm *.o -f; rm scanner -f
