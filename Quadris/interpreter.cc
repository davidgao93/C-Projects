/*
* Interpreter.cc
* Interpreter.cc fully implements the methods in Interpreter.h
*/

#include <iostream>
#include <string>
#include "interpreter.h"

const int COMMANDS = 14;
const int CHEATS = 3;
using namespace std;

// Constructor
Interpreter::Interpreter() {
	usableComm = new TrieNode;
	
	nch = CHEATS;
	nc = COMMANDS;
	
	cheatCommands = new string[nch];
	Commands = new string[nc+nch];
	Commands[0] = "left";
	Commands[1] = "right";
	Commands[2] = "down";
	Commands[3] = "clockwise";
	Commands[4] = "counterclockwise";
	Commands[5] = "drop";
	Commands[6] = "levelup";
	Commands[7] = "leveldown";
	Commands[8] = "restart";
	Commands[9] = "cheats";
	Commands[10] = "rename";
	Commands[11] = "help";
	Commands[12] = "man";
	Commands[13] = "shadow";
	Commands[14] = "clear";
	Commands[15] = "softreset";
	Commands[16] = "up";
	
	cheatCommands[0] = "clear";
	cheatCommands[1] = "softreset";
	cheatCommands[2] = "up";
	
	for (int i = 0; i < nc; i++) {
		usableComm = insert(usableComm, Commands[i]);
	}
}

// Destructor
Interpreter::~Interpreter() {
	delete [] Commands;
	delete [] cheatCommands;
	delete usableComm;
}

// Helper functions
// Function converts a string to an integer
int strToInt(string s) {
	int n = 0;
	for (int i = 0; i < s.length(); i++) {
		n = n * 10 + (s[i] - '0');
	}
	return n;
}

// Methods for functionality
// Multiplier method
// ex. 2levelup - increases our level by 2 instead of 1
// ex2. 3drop - drop the current block, the next block, and the next
void Interpreter::Multiplier(int &mult, string &s) {
	int t = 0;
	string tmp = "";

	for (int i = 0; i < s.length(); i++) {
		if (s[i] >= '0' && s[i] <= '9') {
			tmp += s[i];		// String concatenates the number to the string
		} else {
			t = i;	// Sets the command position to be at the ith position
			break;
		}
	}

	for (int i = t; i < s.length(); i++) {
		if (s[i] < 'a' || s[i] > 'z') { s = ""; return; }
	}

	if (tmp == "") { return; }

	if (t != 0) {
		s = s.substr(t, s.length()-t);
		mult = strToInt(tmp);
	} else {
		s = "";
	}
}

// Command reference ... command
void Interpreter::listCommands() {
	cout << "The following commands are available:" << endl;
	for (int i = 0; i < nc; i++) {
		cout << i+1 << ". " << Commands[i] << endl;
	}

	cout << endl;
	cout << "Commands can take a multiplier prefix, indicating that that command should be ";
	cout << "executed some number of times. For example 3ri means move to the right by three cells." << endl;
	cout << endl;
	cout << "Using manual pages with the following 'man [COMMANDNAME]' ";
	cout << "gives a detailed explanation of each command." << endl;
	cout << endl;
}

void Interpreter::manCommand(string cmd) {
	if (cmd == Commands[0]) { // left
		cout << Commands[0] << endl;
		cout << "Moves the current block one cell to the left." << endl;
		cout << "If this is not possible (left edge of the board, or block in the way), the command has no effect." << endl;
	} else if (cmd == Commands[1]) { // right
		cout << Commands[1] << endl;
		cout << "Moves the current block one cell to the right." << endl;
		cout << "If this is not possible (right edge of the board, or block in the way), the command has no effect." << endl;
	} else if (cmd == Commands[2]) { // down
		cout << Commands[2] << endl;
		cout << "Moves the current block one cell downward." << endl;
		cout << "If this is not possible (bottom edge of the board, or block in the way), the command has no effect." << endl;
	} else if (cmd == Commands[3]) { // clockwise
		cout << Commands[3] << endl;
		cout << "Rotates the block 90 degrees clockwise about the bottom left corner." << endl;
		cout << "If rotation cannot be accomplished without coming into contact with existing " << endl;
		cout << "blocks the command has no effect." << endl;
	} else if (cmd == Commands[4]) { // counterclockwise
		cout << Commands[4] << endl;
		cout << "Rotates the block 90 degrees counterclockwise about the bottom left corner. ";
		cout << "If the rotation cannot be accomplished without coming into contact with existing blocks ";
		cout << "existing blocks or going outside the game board, the command has no effect." << endl;
	} else if (cmd == Commands[5]) { // drop	
		cout << Commands[5] << endl;
		cout << "Drops the current block down until it one of its bottom cells rests on an " << endl;
		cout << "existing block or on the bottom edge of the board. The command also triggers " << endl;
		cout << "the next block to appear on the game board and generates a new next block" << endl;
		cout << "which is displayed in the appropriate place." << endl;
	} else if (cmd == Commands[6]) { // levelup
		cout << Commands[6] << endl;
		cout << "Increases the difficulty level of the game by one." << endl;
		cout << "The block showing as next still comes next, but subsequent blocks are generated using the new level." << endl;
		cout << "If there is no higher level, this command has no effect." << endl;
	} else if (cmd == Commands[7]) { // leveldown
		cout << Commands[7] << endl;
		cout << "Decreases the difficulty level of the game by one." << endl;
		cout << "The block showing as next still comes next, but subsequent blocks are generated using the new level." << endl;
		cout << "If there is no lower level, this command has no effect." << endl;
	} else if (cmd == Commands[8]) { // restart
		cout << Commands[8] << endl;
		cout << "Clears the board and starts a new game." << endl;
	} else if (cmd == Commands[9]) { // cheats
		cout << Commands[9] << endl;
		cout << "Toggles cheat mode. To find out more, type 'help' and 'man' the cheat you want to activate." << endl;
		cout << "WARNING: CHEATING WILL DISABLE ALL HIGH SCORING FROM BEING PRESERVED." << endl;
	} else if (cmd == Commands[10]) { // rename
		cout << Commands[10] << " [old command name] [new command name]" << endl;
		cout << "WARNING: NO MULTIPLIERS ALLOWED FOR THIS COMMAND" << endl;
		cout << "It is possible to rename any command with this." << endl;
	} else if (cmd == Commands[11]) { // help
		cout << Commands[11] << endl;
		cout << "WARNING: NO MULTIPLIERS ALLOWED FOR THIS COMMAND" << endl;
		cout << "Displays all currently available commands to the user. Changes depending on if cheatmode is on or not." << endl;
	} else if (cmd == Commands[12])	{ // man
		cout << Commands[12] << " [command]" << endl;
		cout << "Gives a more accurate description of the command." << endl;
	}  else if (cmd == Commands[13]) { // man
		cout << Commands[13] << endl;
		cout << "Generates a shadow block preview of where the current block will land." << endl;
	} else if (cmd == cheatCommands[0]) { // clear
		cout << cheatCommands[0] << " [number of row]" << endl;
		cout << "Clears a row, remember that rows are from 1-15, and does not include the 3 reserved at the top. " << endl;
	} else if (cmd == cheatCommands[1]) { // softreset
		cout << cheatCommands[1] << endl;
		cout << "Clears the board and starts a new game, but current score is preserved." << endl;
	} else if (cmd == cheatCommands[2]) { // up
		cout << cheatCommands[2] << endl;
		cout << "Moves the current block one cell up." << endl;
		cout << "If this is not possible (top of the board, or block in the way), the command has no effect." << endl;
	} else {
		cerr << "Command entered isn't a valid command. Syntax: man [command name]" << endl;
		cerr << "The valid list of commands are as follows: " << endl;
		for (int i = 0; i < nc; i++) {
			cerr << i+1 << ". " << Commands[i] << endl;
		}
	}
	cout << endl;
}

// Cheat handlers
void Interpreter::addCheats() {
	for (int i = 0; i < nch; i++) {
		usableComm = insert(usableComm, cheatCommands[i]);
	}
	nc += nch;
}

void Interpreter::removeCheats() {
	for (int i = 0; i < nch; i++) {
		usableComm = remove(usableComm, cheatCommands[i]);
	}
	nc -= nch;
}

// Rename command
void Interpreter::rename(string command, string name) {
	bool found = 0;
	bool nameInUse = 0;

	for (int i = 0; i < nc; i++) {
		if (Commands[i] == name) {
			nameInUse = 1;
		}
	}

	if (nameInUse) {
		cout << name << " already exists as a valid command. Try something else." << endl;
		cout << endl;
		return;
	}

	for (int i = 0; i < name.length(); i++) {
		if (name[i] < 'a' || name[i] > 'z')	{
			cout << "Commands can only be lower-case characters. Please try a different name." << endl;
			cout << endl;
			return;
		}
	}

	string search = find(usableComm, name, "");

	for (int i = 0; i < nc; i++) {
		if (Commands[i] == command) {
			found = 1;
			if (!nameInUse && (!search.length() || search[search.length()-1] != ',')) {
				Commands[i] = name;
			}
		}
	}

	if (search.length() > 0 && search[search.length()-1] == ',') {
		cout << name << " is the prefix of an existing command, only a non-ambiguous command rename is allowed." << endl;
		cout << endl;
		return;
	}

	if (!found) {
		cout << command << " was not found. Cannot proceed with rename." << endl;
		cout << endl;
		return;
	}

	usableComm = remove(usableComm, command);
	usableComm = insert(usableComm, name);

	cout << command << " has been renamed to " << name << "." << endl;		//Outputs command succesfully renamed message
	cout << endl;
}

// Starts the game of quadris
void Interpreter::initQuadris(int seed, bool g, string sfile, int initLvl) {
	string input, command, name, cmdFound;
	int mult = 1;
	bool moved, isLegal;

	Board *quadris = new Board(seed, g, initLvl, sfile);
	cout << quadris;

	while(cin >> input) {
		Multiplier(mult, input);
		
		if (input != "") {
			cmdFound = find(usableComm, input, "");
		}

		if (input == "" || cmdFound.length() == 0 || cmdFound[cmdFound.length()-1] != ',') {
			cout << "No command, or not a valid command." << endl;
			cout << "Type 'help' for a list of valid commands." << endl;
			cout << endl;
			continue;
		}

		cmdFound = cmdFound.substr(0, cmdFound.length()-1);

		if (cmdFound.find(',') != string::npos) { // Using our find function, the , was inserted before the last position of string. So it's ambiguous.
			cout << "Command is ambiguous" << endl;
			cout << "Here are the matching commands:" << endl;
			cout << cmdFound << endl;
			cout << "Use 'man [COMMANDNAME]' command to find out more about a specific command." << endl;
			cout << endl;
		} else {
			moved = 0;
			for (int i = 0; i < mult; i++) {
				isLegal = 0;
				if (cmdFound == Commands[0]) {
					isLegal |= quadris->left(i);
				} else if (cmdFound == Commands[1]) {
					isLegal |= quadris->right(i);
				} else if (cmdFound == Commands[2]) {
					isLegal |= quadris->down(i);
				} else if (cmdFound == Commands[3]) {
					isLegal |= quadris->rotate(1, i);
				} else if (cmdFound == Commands[4]) {
					isLegal |= quadris->rotate(0, i);
				} else if (cmdFound == Commands[5]) {
					isLegal |= quadris->drop();
				} else if (cmdFound == Commands[6]) {
					isLegal |= quadris->levelUp();
				} else if (cmdFound == Commands[7]) {
					isLegal |= quadris->levelDown();
				} else if (cmdFound == Commands[8]) {
					if (quadris->cheatMode) {
						removeCheats();
					}
					quadris->reset();
					moved = 1;
					break;
				} else if (cmdFound == Commands[9]) {
					quadris->switchCheats();
					if (quadris->cheatMode) {
						cout << "Cheats enabled." << endl;
						addCheats();
					} else {
						cout << "Cheats disabled." << endl;
						removeCheats();
					}
					cout << endl;
					goto next;
				} else if (cmdFound == Commands[10]) {
					cin >> command >> name;
					rename(command, name);
					goto next;
				} else if (cmdFound == Commands[11])	{
					listCommands();
					goto next;
				} else if (cmdFound == Commands[12])	{
					cin >> command;
					manCommand(command);
					goto next;
				} else if (cmdFound == Commands[13])	{
					quadris->toggleShadow();
					if (quadris->shadowMode) {
						cout << "Shadow guidance enabled." << endl;
					} else {
						cout << "Shadow guidance disabled." << endl;
					}
					goto next;
				}
				
				if (nc > COMMANDS) {
					if (cmdFound == Commands[COMMANDS]) {
						int r;
						cin >> r;
						quadris->clear(18-r, 1);
					} else if (cmdFound == Commands[15]) {
						quadris->softreset();
					} else if (cmdFound == Commands[16]) {
						isLegal |= quadris->up(i);		
					}
				}
				
				moved = moved | isLegal;

				if (!isLegal) {					
					break;
				}
			}

			quadris->legal = moved;

			if (quadris->gameOver()) {
				quadris->gfxOver();
				cout << "                                      " << endl;
				cout << " ___ ___ _____ ___    ___ _ _ ___ ___ " << endl;
				cout << "| . | .'|     | -_|  | . | | | -_|  _|" << endl;
				cout << "|_  |__,|_|_|_|___|  |___|\\_/|___|_|  " << endl;
				cout << "|___|                                 " << endl;
				cout << "High Score: " << quadris->hiScore << endl;
				cout << endl;
				cout << "New game? (y/n)" << endl;
				cin >> input;
				if (input == "Y" || input == "y") {
					quadris->reset();
				} else {
					goto endgame;
				}
			}
			cout << quadris;
			next:;
			mult = 1;
		}
	}
	endgame:;
	if (quadris->hiScore <= 0) {
		cout << "Too bad! You got a score of 0. Please improve your Quadris skills and come play again!" << endl;
	} else {
		cout << "Congratulations! You reached a high score of: " << quadris->hiScore << "." << endl;
	}
	delete quadris;
}