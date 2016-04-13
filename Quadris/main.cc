/*
* The game of Quadris
* main.cc is the driver file that recognizes the initial parameters of the game
*/

#include <iostream>
#include <cstdlib>
#include "interpreter.h"

using namespace std;

//Main function
int main(int argv, char* argc[]) {
	int seed = rand() % 100;
	string scfile = "sequence.txt";
	int initLvl = 1;
	bool xwflag = 1;
	string s;

	for (int i = 1; i < argv; i++) {
		s = argc[i];
		if (s == "-seed") {
			if (argv <= (i+1) || atoi(argc[i+1]) < 0)	{
				cerr << "No seed or negative seed" << endl;
				return 0;
			} else {
				seed = strToInt(argc[i+1]);
			}
		} else if (s == "-text") {
			xwflag = 0;
		} else if (s == "-scriptfile") {
			scfile = argc[i+1];
		} else if (s == "-startlevel" && strToInt(argc[i+1]) < 4 && strToInt(argc[i+1]) >= 0) {
			initLvl = strToInt(argc[i+1]);
		}
	}

	Interpreter *c = new Interpreter;
	c->initQuadris(seed, xwflag, scfile, initLvl);
	delete c;
	return 0;
}