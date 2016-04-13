/*
* Block.h
* Block.h implements the Block class, which describes the type of Blocks there are in Quadris.
* In this version, there are a total of 7 types, these are:
* I
* J
* L
* O
* S
* Z
* T
* Each block is further represented by multiple cells are then placed on the grid.
*/

#ifndef __BLOCK_H__
#define __BLOCK_H__
#include "PRNG.h"
#include "window.h"
#include <map>

const int TYPES = 7;
const char blockTypes[TYPES] = {'I', 'J', 'L', 'O', 'S', 'Z', 'T'};

class Block {
	char type;
	int x, y;
	int lv;

  public: 
  
	// Constructors
	Block() { type = (char)32, x = y = lv = -1; }
	Block(char, int, int, int);
	
	// Destructor
	~Block();

	// Methods for graphics

	void drawBlock(Xwindow*, std::map<char, int>);
	void undrawBlock(Xwindow*);
	void undrawNextBlock(Xwindow*);

	// Methods to store block information
	bool p[4][4];		
	void setType(char);			
	char getType();
	void alignBlock();
	
	// Methods used for Coordinates
	void setX(int);
	int getX();	

	void setY(int);
	int getY();

	// Methods for functionality
	int getLevel();
	char generateNewBlock(int, PRNG&);

	friend std::ostream& ::operator<<(std::ostream&, Block*);
};
#endif
