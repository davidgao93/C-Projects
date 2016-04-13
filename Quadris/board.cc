/*
* Board.cc
* Board.cc implements the Board class as described in the Board.h file.
*/

#include <fstream>
#include "board.h"

using namespace std;

// Constructor
Board::Board(int s, bool graphics, int lv, string fi) {
	level = lv;
	legal = 1;
	firstTime = 0;
	blocksLeft = 0;
	currentScore = 0;
	hiScore = 0;
	seed = s;
	gfx = graphics;
	sX = sY = -1;
	currentfile = fi;
	
	if (gfx) {
		initColors();
		quadWind = new Xwindow(600, 600);
		initGrid();
	}

	cSequence = NULL;
	prng = new PRNG;
	
	if (seed) {
		prng->seed(seed);
	}
	
	if (level == 0) {
		readCharSequence(currentfile);
		currentBlock = new Block(generateNewBlock(1, *prng), 0, 0, 0);
		nextBlock = new Block(generateNewBlock(1, *prng), 0, 0, 0);
		loadBlock();
		loadBlock();
	} else {
		currentBlock = new Block(generateNewBlock(level, *prng), 0, 0, level);
		nextBlock = new Block(generateNewBlock(level, *prng), 0, 0, level);
	}
	for (int i = 0; i < BLOCKS; i++) {
		used[i] = 0;
		blockLevel[i] = -1;
	}
	
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			grid[i][j].type = (char)32;
			grid[i][j].lv = grid[i][j].blockID = -1;
		}
	}
}

// Destructor
Board::~Board() {
	delete[] cSequence;
	delete currentBlock;
	delete nextBlock;
	delete quadWind;
	delete prng;
}

// Reset() handles the reset of the board when the game is over. Everything is gone except high score.
void Board::reset() {
	legal = 1;
	firstTime = 0;
	blocksLeft = 0;
	delete[] cSequence;
	cSequence = NULL;
	quadWind->fillRectangle(0, 0, 600, 600, Xwindow::White);
	if (gfx) {
		for (int i = 0; i < cellStack.size(); i++) {
			cellStack[i].second.undraw(quadWind, cellStack[i].first.second, cellStack[i].first.first);
		}
		
		if (level != 1) {
			quadWind->fillRectangle(470, 420, 50, 20, Xwindow::White);
			quadWind->drawString(415, 430, "             1");
		}
	
		if (currentScore != 0) {
			quadWind->fillRectangle(470, 440, 50, 20, Xwindow::White);
			quadWind->drawString(415, 450, "             0");
		}
		currentScore = 0;
		quadWind->fillRectangle(490, 460, 50, 20, Xwindow::White);
		quadWind->drawString(415, 470, "             " + intToString(hiScore));
	}
	
	level = 1;
	
	while(cellStack.size()) {
		cellStack.erase(cellStack.begin()+0);
	}
	
	for (int i = 0; i < BLOCKS; i++) {
		used[i] = 0;
		blockLevel[i] = -1;
	}
	
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			grid[i][j].type = (char)32;
			grid[i][j].lv = grid[i][j].blockID = -1;
		}
	}
	
	delete currentBlock;
	delete nextBlock;
	delete prng;
	
	prng = new PRNG;
	
	if (seed) {
		prng->seed(seed);
	}
	
	currentBlock = new Block(generateNewBlock(level, *prng), 0, 0, level);
	nextBlock = new Block(generateNewBlock(level, *prng), 0, 0, level);
	initGrid();
}

// Grid boundary logic, whether a move can be done, etc.
bool Board::ob(int x, int y) {
	return (x < 0 || x >= ROWS || y < 0 || y >= COLUMNS);
}

bool Board::legalMove() {
	int x = currentBlock->getX();
	int y = currentBlock->getY();
	
	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {
			if (currentBlock->p[i-x][j-y]) {
				if (ob(i, j) || grid[i][j].type != (char)32) {
					return false;
				}
			}
		}
	}
	return true;
}

// Level 0 handling
void Board::readCharSequence(string sfile) {
	ifstream fin(sfile.c_str());
	string s;

	sizeChar = 0;
	nextChar = 0;
	
	while(fin >> s) {
		sizeChar++;
	}

	fin.close();

	fin.open(sfile.c_str());	
	cSequence = new char[sizeChar];

	for (int i = 0; i < sizeChar; i++) {
		fin >> s;
		if (s.length() != 1 || !(s == "L" || s == "T" || s == "S" || s == "I" || s == "O" || s == "J" || s == "Z")) {
			cSequence[i] = '*';
		} else {
			cSequence[i] = s[0];
		}
	}
}

bool Board::gameOver() {
	return blocksLeft||!legalMove();
}

void Board::gfxOver() {
	quadWind->fillRectangle(0, 0, 400, 600, Xwindow::White);
	quadWind->drawString(200, 250, "GAME OVER");
	quadWind->drawString(200, 300, "New game? (y/n)");
}

// Block ID methods, for scoring purposes

int Board::nextID() {
	for (int i = 0; i < BLOCKS; i++) {
		if (!used[i]) {
			used[i] = 1;
			return i;	
		}
	}
}

void Board::assignBlock(int id, int lvl) {
	blockLevel[id] = lvl;
	int x = currentBlock->getX();
	int y = currentBlock->getY();
	
	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {
			if (currentBlock->p[i-x][j-y]) {
				grid[i][j].blockID = id;
				grid[i][j].lv = lvl;
			
				if (gfx) {
					cellStack.push_back(make_pair(make_pair(i, j), grid[i][j]));
					cellStack[cellStack.size()-1].second.draw(quadWind, 
															cellStack[cellStack.size()-1].first.second, 
															cellStack[cellStack.size()-1].first.first, 
															Colors[cellStack[cellStack.size()-1].second.type]);
				}
			}
		}
	}
}

// Block movement handlers
bool Board::left(int t) {
	currentBlock->setY(currentBlock->getY()-1);
	
	if (!legalMove()) {
		currentBlock->setY(currentBlock->getY()+1);
		return false;
	}
	
	if (t == 0 && gfx) {
		currentBlock->setY(currentBlock->getY()+1);
		undraw();
		currentBlock->setY(currentBlock->getY()-1);
	}
	
	return true;
}

bool Board::right(int t) {
	currentBlock->setY(currentBlock->getY()+1);

	if (!legalMove()) {
		currentBlock->setY(currentBlock->getY()-1);
		return false;
	}		

	if (t == 0 && gfx) {
		currentBlock->setY(currentBlock->getY()-1);
		undraw();
		currentBlock->setY(currentBlock->getY()+1);
	}

	return true;
}

bool Board::down(int t) {
	currentBlock->setX(currentBlock->getX()+1);			// Move the block down

	if (!legalMove()) {
		currentBlock->setX(currentBlock->getX()-1);		// Move the block back up
		return false;
	}
	
	if (t == 0 && gfx) {
		currentBlock->setX(currentBlock->getX()-1);		// Move the block back up (to undraw it)
		undraw();
		currentBlock->setX(currentBlock->getX()+1);		// Move the block down
	}
	
	return true;
}

bool Board::drop() {
	int numDown = 0;

	while(down(numDown++));	
	moveBlock(0);
	assignBlock(nextID(), currentBlock->getLevel());
	canclr();
	loadBlock();

	if (gameOver() || (level == 0 && blocksLeft)) {
		return false;
	} else {
		return true;
	}
}

bool Board::rotate(bool dir, int t) {
	bool tmp[4][4], tmp2[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tmp2[i][j] = currentBlock->p[i][j];
		}
	}

	if (dir) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tmp[i][j] = currentBlock->p[3-j][i];
			}
		}
	} else {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tmp[i][j] = currentBlock->p[j][3-i];
			}
		}
	}

	for (int i = 0; i < 4; i++)	{
		for (int j = 0; j < 4; j++) {
			currentBlock->p[i][j] = tmp[i][j];
		}
	}

	currentBlock->alignBlock();
	if (legalMove()) {
		if (t == 0) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					currentBlock->p[i][j] = tmp2[i][j];
				}
			}
			undraw();
		}
		
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				currentBlock->p[i][j] = tmp[i][j];
			}
		}
		currentBlock->alignBlock();
		return true;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			currentBlock->p[i][j] = tmp2[i][j];
		}
	}

	return false;
}

void Board::moveBlock(int mode) {
	int x = currentBlock->getX(), y = currentBlock->getY();

	if (mode == 0 && legal) {
		draw();
	}

	for (int i = x; i < x + 4; i++) {
		for (int j = y; j < y + 4; j++) {
			int px = i - x, py = j - y;
			if (currentBlock->p[px][py]) {
				if (mode == 0) {
					grid[i][j].type = currentBlock->getType();
				} else {
					grid[i][j].type = (char)32;
				}
			}
		}
	}
}

// Block loading handler

void Board::loadBlock() {
	delete currentBlock;
	currentBlock = new Block(nextBlock->getType(), nextBlock->getX(), nextBlock->getY(), nextBlock->getLevel());	

	char nextType;
	
	if (level == 0) {
		while(cSequence[nextChar] == '~') {
			cerr << "No block given." << endl;
			nextChar++;
		}

		nextType = cSequence[nextChar]; 
	} else {
		nextType = nextBlock->generateNewBlock(level, *prng);
	}

	if (gfx && nextBlock->getType() != nextType) {
		nextBlock->undrawNextBlock(quadWind);
	}

	delete nextBlock;

	if (level == 0 && (nextChar >= sizeChar)) {
		nextBlock = new Block();
		
		if (nextChar >= sizeChar) {
			blocksLeft = 1;	
		}

		nextChar++;
	} else {
		nextBlock = new Block((level==0?cSequence[nextChar++]:nextType), 0, 0, level);
	}
}

// Score, level handlers
bool Board::levelUp() {
	if (level < LEVELS) {
		level++;
		if (gfx) {	
			quadWind->fillRectangle(470, 420, 50, 20, Xwindow::White);
			quadWind->drawString(415, 430, "             " + intToString(level));
		}
		return true;
	}
	return false;
}

bool Board::levelDown() {
	if (level > 0) {
		--level;

		if (level == 0 && !firstTime) {
			firstTime = 1;
			readCharSequence(currentfile);
		}
		
		if (gfx) {	
			quadWind->fillRectangle(470, 420, 50, 20, Xwindow::White);
			quadWind->drawString(415, 430, "             " + intToString(level));
		}
		return true;
	}
	return false;
}

void Board::updateScore(int lvl) {
	if (!cheatMode) {
		cout << lvl << endl; 
		currentScore += (lvl + 1)*(lvl + 1);

		if (currentScore > hiScore) {
			hiScore = currentScore;

			if (gfx) {	
				quadWind->fillRectangle(490, 460, 50, 20, Xwindow::White);
				quadWind->drawString(415, 470, "             " + intToString(hiScore));
			}
		}

		if (gfx) {	
			quadWind->fillRectangle(470, 440, 50, 20, Xwindow::White);
			quadWind->drawString(415, 450, "             " + intToString(currentScore));
		}
	}
}

// Block clear logic
void Board::canclr() {
	for (int i = 3; i < ROWS; i++) {
		bool shouldClear = 1;
		for (int j = 0; j < COLUMNS; j++) {
			if (grid[i][j].type == (char)32) {
				shouldClear = 0;
				break;
			}
		}

		if (shouldClear) {
			clear(i, 0);
		}
	}
}

void Board::clear(int r, int c) {
	if (!c) {
		updateScore(level);
	}
	
	
	for (int i = r; i >= 3; --i) {
		for (int j = 0; j < COLUMNS; j++) {
			grid[i][j].type = grid[i-1][j].type;
			grid[i][j].lv = grid[i-1][j].lv;
			grid[i][j].blockID = grid[i-1][j].blockID;
		}
	}
	
	if (gfx) {
		int i;
		for (i = 0; i < cellStack.size(); i++) {
			if (cellStack[i].first.first < r) {
				cellStack[i].second.undraw(quadWind, cellStack[i].first.second, cellStack[i].first.first); 
				cellStack[i].first.first++;
			} else if (cellStack[i].first.first == r) {
				cellStack[i].second.undraw(quadWind, cellStack[i].first.second, cellStack[i].first.first);
				cellStack.erase(cellStack.begin()+i);
				--i;
			}
		}
		
		for (int i = 0; i < cellStack.size(); i++) {
			if (cellStack[i].first.first <= r) {
				cellStack[i].second.draw(quadWind, 
										cellStack[i].first.second, 
										cellStack[i].first.first, 
										Colors[cellStack[i].second.type]);
			}
		}
		if (shadowMode) {
			findShadow(sX, sY);
			if (sX <= r - 3 && c) {
				undrawShadow(0);
			}
		}
	}
	
	int tmpused[BLOCKS];

	for (int i = 0; i < BLOCKS; i++) {
		tmpused[i] = 0; 
	}

	for (int i = 3; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			if (grid[i][j].type != (char)32) {
				tmpused[grid[i][j].blockID] = 1;
			}
		}
	}
	
	for (int i = 0; i < BLOCKS; i++) {
		if (used[i] && !tmpused[i]) {
			if (!cheatMode) {	
				updateScore(blockLevel[i]);
			}
			blockLevel[i] = -1;
		}
		used[i] = tmpused[i];
	}
}

// Methods used for cheats
void Board::switchCheats() {
	cheatMode ^= 1;
	initGrid();
}

bool Board::up(int t) {	
	currentBlock->setX(currentBlock->getX()-1);
	
	if (!legalMove()) {
		currentBlock->setX(currentBlock->getX()+1);
		return false;
	}
	
	if (t == 0 && gfx) {
		currentBlock->setX(currentBlock->getX()+1);	
		undraw();	
		currentBlock->setX(currentBlock->getX()-1);
	}
	
	return true;
}

void Board::softreset() {	
	if (gfx) {	
		for (int i = 0; i < cellStack.size(); i++) {
			cellStack[i].second.undraw(quadWind, cellStack[i].first.second, cellStack[i].first.first);
		}
		findShadow(sX, sY);
	}

	while(cellStack.size())	{
		cellStack.erase(cellStack.begin()+0);
	}

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			grid[i][j].type = (char)32;
			grid[i][j].lv = grid[i][j].blockID = -1;
		}
	}

	if (sX < (ROWS) - 4) {
		undrawShadow(0);
	}

	for (int i = 0; i < BLOCKS; i++) {
		used[i] = 0;
	}
}

// Methods for standard block drawing
void Board::initColors() {
	int color = 2;
	for (int i = 0; i < BLOCKS; i++) {
		Colors[blockTypes[i]] = color++;
	}
}

void Board::initGrid() {
	quadWind->drawString(230, 100, "QUADRIS");
	int clist = 14;
	if (cheatMode) {
		clist = 17;
	}
	quadWind->drawString(10, 100, "Commands:");
	string comms[] = {"left","right","down","clockwise","counterclockwise","drop","levelup",
					  "leveldown","restart","cheats","rename","help","man","shadow","clear",
					  "softreset","up"};
	quadWind->drawString(415, 430, "Level:       " + intToString(level));
	quadWind->drawString(415, 450, "Score:       " + intToString(currentScore));
  	quadWind->drawString(415, 470, "High Score:  " + intToString(hiScore));
	for (int i = 0; i < clist; i++) {
		quadWind->drawString(10, 130 + 15 * i, intToString(i+1) + ". " + comms[i]);
	}
	if (clist == 14) {
		quadWind->fillRectangle(10, 325, 100, 50, Xwindow::White);
	}
	
	for (int i = 0; i <= ROWS; i++) {
		quadWind->fillRectangle(125, 25 * i + 125, 250, 1, Xwindow::Black);
	}
	for (int i = 0; i <= COLUMNS; i++) {
		quadWind->fillRectangle(25 * i + 125, 125, 1, 450, Xwindow::Black);
	}
	
	quadWind->drawString(450, 115, "Next Block");
	for (int i = 0; i < 5; i++) {
		quadWind->fillRectangle(40 * i + 400, 125, 1, 160, Xwindow::Black);
		quadWind->fillRectangle(400, 40 * i + 125, 160, 1, Xwindow::Black); 
	}
}

void Board::draw() {
	if (gfx) {
		int x = currentBlock->getX();
		int y = currentBlock->getY();
		int clr = Colors[currentBlock->getType()];
		
		for (int i = x; i < x + 4; i++) {
			for (int j = y; j < y + 4; j++) {
				if (currentBlock->p[i-x][j-y]) {
					quadWind->fillRectangle(25 * j + 125 + 1, 25 * i + 125 + 1, 23, 23, clr);
				}
			}
		}
		if (shadowMode) {
			drawShadow();
		}
	}
}

void Board::undraw() {
	if (gfx) {
		int x = currentBlock->getX();
		int y = currentBlock->getY();  
		
		for (int i = x; i < x + 4; i++) {
			for (int j = y; j < y + 4; j++) {
				if (currentBlock->p[i-x][j-y]) {
					quadWind->fillRectangle(25 * j + 125 + 1, 25 * i + 125 + 1, 23, 23, Xwindow::White);
				}
			}
		}
		if (shadowMode) {
			undrawShadow(1);
		}
	}
}

// Methods used for shadow block
void Board::toggleShadow() {
	shadowMode ^= 1;
	if (shadowMode) {
		drawShadow();
	} else {
		undrawShadow(1);
	}
}

void Board::findShadow(int &x, int &y) {
	int numMoved = 0;

	while(down(1)) {	
		numMoved++;
	}
	
	for (int i = 0; i < numMoved; i++) {
		up(1);
	}

	x = currentBlock->getX() + numMoved;
	y = currentBlock->getY();
}

void Board::drawShadow() {
	if (gfx) {	
		findShadow(sX, sY);
		if (sX == currentBlock->getX() && sY == currentBlock->getY()) { return; }
		int shadowColor = Xwindow::Black;
		for (int i = sX; i < sX + 4; i++) {		
			for (int j = sY; j < sY + 4; j++) {
				if (currentBlock->p[i-sX][j-sY]) {
					quadWind->fillRectangle(25 * j + 125 + 5, 25 * i + 125 + 5, 15, 15, shadowColor);
				}
			}
		}
	}
}

void Board::undrawShadow(bool n) {
	if (gfx) { 	
		if (n) {
			findShadow(sX, sY);
			if (sX == currentBlock->getX() && sY == currentBlock->getY()) { return; }
		}
		for (int i = sX; i < sX + 4; i++) {		
			for (int j = sY; j < sY + 4; j++) {
				if (currentBlock->p[i-sX][j-sY]) {
					quadWind->fillRectangle(25 * j + 125 + 5, 25 * i + 125 + 5, 15, 15, Xwindow::White);
				}
			}
		}
		if (n == 0) { draw();}
	}
}

// Helper functions
string Board::intToString(int n) {
	if (n == 0) {
		return "0";
	}
	
	string tmp = "";
	string fi = "";
	
	while(n) {
		tmp += (char)((n%10) + (int)'0');
		n /= 10;
	}
	
	for (int i = tmp.length()-1; i >= 0; --i) {
		fi += tmp[i];
	}
	
	return fi;
}

ostream& operator<<(ostream &out, Board* b) {
	out << "Level:   \t" << b->level << endl;
	out << "Score:   \t" << b->currentScore << endl;
	out << "High Score:\t" << b->hiScore << endl;
	out << "----------" << endl;

	b->moveBlock(0);

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			out << (b->grid[i][j]).type;
		}
		out << endl;
	}

	b->moveBlock(1);
	
	if (b->level != 0 && b->nextBlock->getType() == (char)32) {
		b->nextBlock = new Block(b->generateNewBlock(b->level, *(b->prng)), 0, 0, b->level);
	}

	out << "----------" << endl;
	out << "Next:" << endl;
	out << b->nextBlock;
	
	if (b->gfx) {
		b->nextBlock->drawBlock(b->quadWind, b->Colors);
	}
	return out;
}