/* 
* Block.cc
* Block.cc implements methods found in Block.h
*/

#include "block.h"

using namespace std;

// Constructor
Block::Block(char t, int newX, int newY, int lvl) {
	type = t, x = newX, y = newY, lv = lvl;

	for(int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			p[i][j] = 0;
		}
	}

	if (type == 'I') {
		p[3][0] = 1;
		p[3][1] = 1;
		p[3][2] = 1;
		p[3][3] = 1;
	} else if (type == 'J') {
		p[2][0] = 1;
		p[3][0] = 1;
		p[3][1] = 1;
		p[3][2] = 1;
	} else if (type == 'L') {
		p[2][2] = 1;
		p[3][0] = 1;
		p[3][1] = 1;
		p[3][2] = 1;
	} else if (type == 'O')	{
		p[2][0] = 1; 
		p[2][1] = 1; 
		p[3][0] = 1; 
		p[3][1] = 1;
	} else if (type == 'S') {
		p[3][0] = 1;
		p[3][1] = 1; 
		p[2][1] = 1; 
		p[2][2] = 1;
	} else if (type == 'Z')	{
		p[2][0] = 1; 
		p[2][1] = 1; 
		p[3][1] = 1; 
		p[3][2] = 1;
	} else if (type == 'T') {
		p[2][0] = 1;
		p[2][1] = 1; 
		p[2][2] = 1; 
		p[3][1] = 1;
	}
}

// Destructor
Block::~Block() {}

// Accessors
int Block::getX() {
	return x;
}

int Block::getY() {
	return y;
}

char Block::getType() {
	return type;
}

int Block::getLevel() {
	return lv;
}

// Setters
void Block::setX(int x1) {
	x = x1;
}

void Block::setY(int y1) {
	y = y1;
}

void Block::setType(char t) {
	type = t;
} 

// Block alignment logic
void Block::alignBlock() {
	bool isEmpty = 1;

	while(1) {
		isEmpty = 1;
		for(int i = 0; i < 4; i++) {
			if (p[i][0]) {
				isEmpty = 0;
				break;
			}
		}

		if (!isEmpty) {
			break;
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				p[i][j] = p[i][j+1];
			}
		}

		for (int i = 0; i < 4; i++) {
			p[i][3] = 0;
		}
	}

	while(1) {
		isEmpty = 1;
		for (int i = 0; i < 4; i++) {
			if (p[3][i]) {
				isEmpty = 0;
				break;
			}
		}
		
		if (!isEmpty) {
			break;
		}

		for (int i = 3; i > 0; --i) {
			for (int j = 0; j < 4; j++) {
				p[i][j] = p[i-1][j];
			}
		}

		for (int i = 0; i < 4; i++) {
			p[0][i] = 0;
		}
	}
}

// Block generation logic
char Block::generateNewBlock(int lvl, PRNG& prng) {
	int r;
	char t;
	
	if (lvl == 1) {
		r = prng(11);

		if (r == 0) {
			t = 'S';
		} else if (r == 1) {
			t = 'Z';
		} else {	
			r /= 2;
			--r;
			t = blockTypes[r];
		}
	} else if (lvl == 2) {
		t = blockTypes[prng(6)];
	} else if (lvl == 3) {
		r = prng(8);

		if (r == 0 || r == 1) {
			t = 'S';
		} else if (r == 2 || r == 3) {
			t = 'Z';
		} else {
			r -= 4;
			t = blockTypes[r];
		}
	}
	return t;
}

void Block::drawBlock(Xwindow* w, map<char, int> c) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (p[i][j]) {
				w->fillRectangle(40 * j + 400 + 1, 40 * i + 125 + 1, 38, 38, c[type]);
			}
		}
	}
}

void Block::undrawBlock(Xwindow* w) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (p[i][j]) {
				w->fillRectangle(40 * j + 70 + 1, 40 * i + 70 + 1, 38, 38, Xwindow::White);
			}
		}
	}
}

void Block::undrawNextBlock(Xwindow* w) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (p[i][j]) {
				w->fillRectangle(40 * j + 400 + 1, 40 * i + 125 + 1, 38, 38, Xwindow::White);
			}
		}
	}
}

ostream& operator<<(ostream& out, Block* bl) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (bl->p[i][j]) {
				goto next;
			}
		}
		continue;
		next:;
		for (int j = 0; j < 4; j++) {
			if(bl->p[i][j]) {
				out << bl->type;
			} else {
				out << (char)32;
			}
		}
		out << endl;
	}
	
	out << endl;
	return out;
}