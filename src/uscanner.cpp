/*
	nicole cranon <nicole.cranon@ucdenver.edu>
	csci 4640, fall 2015
	homework 4 - universal scanner
 */

#include "uscanner.h"
#include <iostream>

namespace scanner {

	std::vector<std::unordered_map<std::string, int> > T;
	std::vector<std::unordered_map<std::string, std::string> > actionTable;
	std::unordered_map<int, std::string> terminalState;

	std::ifstream infile;

	std::string action_type[] = {
		[Error] = "Error",
		[MoveAppend] = "MoveAppend",
		[MoveNoAppend] = "MoveNoAppend",
		[HaltAppend] = "HaltAppend",
		[HaltNoAppend] = "HaltNoAppend",
		[HaltReuse] = "HaltReuse"
	};

	void readTransitionTable (const char* const transitionTable, 
		std::vector<std::unordered_map<std::string, int>> & T,
		std::unordered_map<int, std::string>& termState) {

		std::ifstream readFile;
		readFile.open(transitionTable);

		if (!readFile.good()) {
			std::cerr << "\nError reading transition table " << transitionTable << ".\n";
			exit (1);
		}

		unsigned state = 0,
			nextState = 0,
			states = 0,
			terminalStatesStart = 0,
			symbolTypes = 0;

		std::string token,
			temp,
			garbage;


		/*get states label*/
		readFile >> garbage;
		readFile >> states; 
		/*get eol and any garbage*/ 
		getline(readFile, garbage);

		/*get beginning of terminal states */
		readFile >> garbage;
		readFile >> terminalStatesStart;
		/*get eol and any garbage*/ 
		getline(readFile, garbage);

		/*get symbolTypes label*/
		readFile >> garbage;
		readFile >> symbolTypes; 
		/*get eol and any garbage*/ 
		getline(readFile, garbage);

		if (T.size() != states - terminalStatesStart) {
			T.resize(states - terminalStatesStart);
		}
		
		// std::cout << "\nSymbols\n";

		std::vector<std::string> symbolTypeArr (symbolTypes);
		for (unsigned i = 0; i < symbolTypes; ++i){
			readFile >> temp;
			symbolTypeArr[i] = temp;
			// std::cout << temp << ' '; 
		} getline (readFile, garbage);

		// std::cout << '\n';
	
		/* read transitions for non terminal states */
		std::unordered_map<std::string, int> symbolNextState;	
		for (unsigned i = 0; i < terminalStatesStart; ++i) {
			for (unsigned j = 0; j < symbolTypes; ++j) {
				readFile >> nextState; 
				symbolNextState [symbolTypeArr[j]] = nextState;
				// std::cout << /*symbolTypeArr[j] << ' ' << */symbolNextState[symbolTypeArr[j]] << ' ';
			}

			getline(readFile, garbage);
			// std::cout << '\n';

			T[state] = symbolNextState;
			++state;
		}

		/* read terminal states and their corresponding tokens*/
		for (unsigned i = terminalStatesStart; i < states; ++i) {
			readFile >> temp;
			getline (readFile, garbage);
			termState[state] = temp;
			// std::cout << '\n' << state << termState[state] << '\n';
			++state;
		}

		readFile.close();
	}

	void lookupCode (int state, char c, int& tokenCode) {
		switch (c) {
			case 'a' ... 'z' :
			case 'A' ... 'Z' :
				tokenCode = T[state]["Letter"];
				// std::cout << tokenCode;
				break;
			case '0' ... '9':
				tokenCode = T[state]["Digit"];
				// std::cout << tokenCode;
				break;
			case ' ':
			case '\t':
				tokenCode = T[state]["Blank"];
				// std::cout << tokenCode;
				break;
			case '+':
				tokenCode = T[state]["+"];
				// std::cout << tokenCode;
				break;
			case '-':
				tokenCode = T[state]["-"];
				// std::cout << tokenCode;
				break;
			case '=':
				tokenCode = T[state]["="];
				// std::cout << tokenCode;
				break;
			case ':':
				tokenCode = T[state][":"];
				// std::cout << tokenCode;
				break;
			case ',':
				tokenCode = T[state][","];
				// std::cout << tokenCode;
				break;
			case ';':
				tokenCode = T[state][";"];
				// std::cout << tokenCode;
				break;
			case '(':
				tokenCode = T[state]["("];
				// std::cout << tokenCode;
				break;
			case ')':
				tokenCode = T[state][")"];
				// std::cout << tokenCode;
				break;
			case '_':
				tokenCode = T[state]["_"];
				// std::cout << tokenCode;
				break;
			case '\r':
			case '\n':
				tokenCode = T[state]["Eol"];
				// std::cout << tokenCode;
				break;
			default:
				std::cerr << "syntax error";
				tokenCode = T[state]["Others"];
		}
	
	}

	void checkExceptions (int& tokenCode, const std::string& tokenText) {
		/* tokenText contains a reserved word and not an identifier */
		if (tokenText.compare("BEGIN")  == 0 ||
			tokenText.compare("END") == 0 ||
			tokenText.compare("READ") == 0 ||
			tokenText.compare("WRITE") == 0) {

			tokenCode = 0;
			return;
		}
		// if (token.tokenText)
	}

	void read (char& c) {

		c = currentChar();
		consumeChar();
	}

	char currentChar () {
		char c = infile.peek();

		return c;
	}

	void consumeChar () {
		infile.get();
	}

	void clearBuffer (std::string& buffer) {
		buffer = "";
	}

	void lexicalError () {
		// TODO Error correction/recovery
		std::cerr << "\nLexical Error\n";

		exit (1);
	}

	void nextState (int& state, char c) {
		switch (c) {
			case 'a' ... 'z' :
			case 'A' ... 'Z' :
				state = T[state]["Letter"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '0' ... '9':
				state = T[state]["Digit"];
				//std::cout << "State " <<  state << '\n';
				break;
			case ' ':
			case '\t':
				state = T[state]["Blank"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '+':
				state = T[state]["+"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '-':
				state = T[state]["-"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '=':
				state = T[state]["="];
				//std::cout << "State " <<  state << '\n';
				break;
			case ':':
				state = T[state][":"];
				//std::cout << "State " <<  state << '\n';
				break;
			case ',':
				state = T[state][","];
				//std::cout << "State " <<  state << '\n';
				break;
			case ';':
				state = T[state][";"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '(':
				state = T[state]["("];
				//std::cout << "State " <<  state << '\n';
				break;
			case ')':
				state = T[state][")"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '_':
				state = T[state]["_"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '\r':
			case '\n':
				state = T[state]["Eol"];
				//std::cout << "State " <<  state << '\n';
				break;
			default:
				std::cerr << "syntax error";
				state = T[state]["Others"];

		}
	}

	Action action (int state, char c) {
		std::string nextAction;

		switch (c) {
			case 'a' ... 'z' :
			case 'A' ... 'Z' :
				nextAction = actionTable[state]["Letter"];
				// std::cout << "*****Action " <<  actionTable[state]["Letter"] << ' ' <<  c << '\n';
				break;
			case '0' ... '9':
				nextAction = actionTable[state]["Digit"];
				//std::cout << "State " <<  state << '\n';
				break;
			case ' ':
			case '\t':
				nextAction = actionTable[state]["Blank"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '+':
				nextAction = actionTable[state]["+"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '-':
				nextAction = actionTable[state]["-"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '=':
				nextAction = actionTable[state]["="];
				//std::cout << "State " <<  state << '\n';
				break;
			case ':':
				nextAction = actionTable[state][":"];
				//std::cout << "State " <<  state << '\n';
				break;
			case ',':
				nextAction = actionTable[state][","];
				//std::cout << "State " <<  state << '\n';
				break;
			case ';':
				nextAction = actionTable[state][";"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '(':
				nextAction = actionTable[state]["("];
				//std::cout << "State " <<  state << '\n';
				break;
			case ')':
				nextAction = actionTable[state][")"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '_':
				nextAction = actionTable[state]["_"];
				//std::cout << "State " <<  state << '\n';
				break;
			case '\r':
			case '\n':
				nextAction = actionTable[state]["Eol"];
				//std::cout << "State " <<  state << '\n';
				break;
			default:
				std::cerr << "syntax error";
				nextAction = actionTable[state]["Others"];
		}

		// std::cout << "\nAction " << nextAction << '\n';

		if (nextAction.compare(action_type[MoveAppend]) == 0) {
			return MoveAppend;
		}
		if (nextAction.compare(action_type[MoveNoAppend]) == 0){
			return MoveNoAppend;
		}
		if (nextAction.compare(action_type[HaltAppend]) == 0) {
			return HaltAppend;
		}
		if (nextAction.compare(action_type[HaltNoAppend]) == 0) {
			return HaltNoAppend;
		}
		if (nextAction.compare(action_type[HaltReuse]) == 0) {
			return HaltReuse;
		}

		return Error;
	}

	void readActionTable (const char* const actionTableFile) {
		std::ifstream readFile;
		readFile.open(actionTableFile);

		if (!readFile.good()) {
			std::cerr << "\nError reading action table " << actionTableFile << ".\n";
			exit (1);
		}

		unsigned state = 0,
			states = 0,
			symbolTypes = 0;

		std::string token,
			temp,
			garbage;


		/*get states label*/
		readFile >> garbage;
		readFile >> states; 
		/*get eol and any garbage*/ 
		getline(readFile, garbage);

		/*get symbolTypes label*/
		readFile >> garbage;
		readFile >> symbolTypes; 
		/*get eol and any garbage*/ 
		getline(readFile, garbage);

		if (actionTable.size() != states) {
			actionTable.resize(states);
		}
		
		// std::cout << "\nSymbols\n";

		std::vector<std::string> symbolTypeArr (symbolTypes);
		for (unsigned i = 0; i < symbolTypes; ++i){
			readFile >> temp;
			symbolTypeArr[i] = temp;
			// std::cout << temp << ' '; 
		} getline (readFile, garbage);

		// std::cout << '\n';
	
		/* read actions for non terminal states */
		std::unordered_map<std::string, std::string> action;	
		for (unsigned i = 0; i < states; ++i) {
			for (unsigned j = 0; j < symbolTypes; ++j) {
				readFile >> temp; 
				action [symbolTypeArr[j]] = temp;
				// std::cout << /*symbolTypeArr[j] << ' ' << */action[symbolTypeArr[j]] << ' ';
			}

			getline(readFile, garbage);
			// std::cout << '\n';

			actionTable[state] = action;
			++state;
		}

		readFile.close();
	}

	void scanner (int& tokenCode, std::string& tokenText) {
		int state = 0; // initial state
		clearBuffer (tokenText);
		std::string garbage = "";

		while (infile.good() && infile.peek() != EOF) {
			Action currentAction;
			// std::cout << "State " << state ;
			currentAction = action (state, currentChar());
			// std::cout << currentChar() << " Action " << action_type[currentAction];
			switch (currentAction) {
				case Error: {
					lexicalError();
					break;
				}
				case MoveAppend: {
					// std::cout << "entered moveAppend state " << state << '\n';
					nextState (state, currentChar());
					// std::cout << "next state after this move moveAppend " << state << '\n';
					tokenText += currentChar();
					consumeChar();
					break;
				}
				case MoveNoAppend: {
					nextState (state, currentChar());
					consumeChar();
					break;
				}
				case HaltAppend : {
					lookupCode (state, currentChar(), tokenCode);
					tokenText += currentChar();
					checkExceptions (tokenCode, tokenText);
					consumeChar ();

					if (tokenCode == 0) {
						scanner (tokenCode, tokenText);
					}

					return;
				}
				case HaltNoAppend : {
					lookupCode (state, currentChar(), tokenCode);
					checkExceptions (tokenCode, tokenText);
					consumeChar ();
					// std::cout << "\nToken Code " << tokenCode << " Token Text " << tokenText << '\n';
					if (tokenCode == 0) {
						scanner (tokenCode, tokenText);
					}

					return;
				}

				case HaltReuse : {
					lookupCode (state, currentChar (), tokenCode);
					checkExceptions (tokenCode, tokenText);
					// std::cout << "\nToken Code " << tokenCode << " Token Text " << tokenText << '\n';

					if (tokenCode == 0) {
						clearBuffer (tokenText);
						scanner (tokenCode, tokenText);
					}

					return;
				}
			}
		} //getline (infile, garbage);
	}
}