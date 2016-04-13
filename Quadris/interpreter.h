/*
* Interpreter.h
* Interpreter.h implements the Interpreter class, which is our command interpreter.
*/

#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__
#include "trie.h"
#include "board.h"
class Interpreter {
	std::string* Commands;
	int nc;
	TrieNode* usableComm;
	
	std::string* cheatCommands;
	int nch;

  public:
	// Constructor
	Interpreter();
	
	// Destructor
	~Interpreter();

	// Methods for functionality
	void Multiplier(int&, std::string&);
	void initQuadris(int, bool, std::string, int);
	void rename(std::string, std::string);
	void listCommands();
	void manCommand(std::string);
	void addCheats();
	void removeCheats();
};

int strToInt(std::string);						// converts a given string to a number (e.g. "123" -> 123)

#endif