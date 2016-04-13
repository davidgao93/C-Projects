/* 
* Board.h
* Board.h is the header file for the Quadris board. 
* In Quadris, the board is composed of 10 columns and 15 rows.
* The board acts as a grid, with each quadris block implemented using cells.
* Board will be visually represented by a TextDisplay as well as a GraphicsDisplay.
*/

#ifndef __BOARD_H__
#define __BOARD_H__
#include "block.h"
#include "cell.h"
#include <map>
#include <vector>
#include "trie.h"

// Constant variables
const int ROWS = 18; // It's technically 15 + 3. We use 3 rows for rotation at the top (reserved).
const int COLUMNS = 10;
const int BLOCKS = 180; // At 18 * 10 cells with on average 4 cells a block, we should have at most 45 blocks.
const int LEVELS = 3;

// Main class: Board
class Board : public Block, public Cell {
	public:
	Block* currentBlock;
	Block* nextBlock;
	Cell grid[ROWS][COLUMNS];
	std::string currentfile;
	char* cSequence;
	int seed; // We use this to generate random numbers for PRNG
	
	// Private members that are used to keep track of the game, score, etc.
	int level, currentScore, hiScore;
	int blockLevel[BLOCKS]; // Stores level the block was generated in
	bool used[BLOCKS]; // Checks if a block has a specific ID used before
	
	PRNG* prng; // Our RNG class instance
	
	// Level 0 handling, a few more things to consider
	bool blocksLeft;
	bool firstTime;
	int nextChar;
	int sizeChar;
  
	// Constructor
	Board(int, bool, int, std::string);
	
	// Destructor
	~Board();
	
	// Vectors & Maps
	std::map <char, int> Colors;
	std::vector< std::pair< std::pair<int, int>, Cell> > cellStack;
	
	// Booleans needed for functionality
	bool legal;
	bool legalMove();
	bool gameOver();
	bool ob(int, int);
	bool left(int);
	bool right(int);
	bool down(int);
	bool drop();
	bool rotate(bool, int); // 1 = clockwise, 0 = counter-clockwise
	bool levelUp();
	bool levelDown();
	
	// Methods needed for functionality
	void reset();
	void readCharSequence(std::string);
	void clear(int, int);
	void canclr();
	void loadBlock();
	void assignBlock(int, int);
	void updateScore(int);
	void moveBlock(int);
	int nextID();
	
	// Methods needed for cheats
	bool cheatMode;			
	void switchCheats();
	bool up(int);
	void softreset();
	
	// Methods needed with shadow of Block
	int sX, sY;
	bool shadowMode;
	void toggleShadow();
	void findShadow(int&,int&);
	void drawShadow();
	void undrawShadow(bool);
	
	// Graphical output
	
	Xwindow* quadWind; // board
	bool gfx;
	bool redraw;
	void initGrid();
	void draw();
	void undraw();
	void initColors();
	void gfxOver();
	
	// Helper functions
	std::string intToString(int);
	
	// Overloaded operators
	
	friend std::ostream&::operator<<(std::ostream&, Board*);
};
#endif