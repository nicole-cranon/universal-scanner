/*
	nicole cranon <nicole.cranon@ucdenver.edu>
	csci 4640, fall 2015
	homework 4 - universal scanner
 */

#include "uscanner.h"
#include <iostream>

int main (int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "\nError: Not enough parameters provided! format is [executable] [programToScan]\n";
		exit(1);
	}

	// argv[1] := the program to be read
	scanner::infile.open (argv[1]);

	if (!scanner::infile.good() || scanner::infile == NULL) {
		std::cerr << "\nError: Could not open provided file!\n";
		exit(1);
	}

	const char* const transitionTable = "./transitionTable";
	scanner::readTransitionTable (transitionTable, scanner::T, scanner::terminalState);

	const char* const actionTable = "./actionTable";
	scanner::readActionTable (actionTable);

	int tokenCode = 0;
	std::string tokenText = "";
	while (scanner::infile.good () &&  scanner::infile.peek() != EOF) {
		scanner::scanner (tokenCode, tokenText);
		std::cout << "\nToken Code " << tokenCode << " Token Text " << tokenText << '\n';
	} 
	scanner::infile.close ();
}
