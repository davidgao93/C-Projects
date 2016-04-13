/*
* Cell.cc
* Cell.cc fully implements all methods found in cell.h
*/

#include "cell.h"

// Constructors
Cell::Cell() {
	type = (char)32;
	lv = blockID = -1;
}

Cell::Cell(char t, int lvl, int ID) {
	type = t;
	lv = lvl;
	blockID = ID;
}

// Destructor
Cell::~Cell() {}

void Cell::draw(Xwindow* w, int x, int y, int c) {
	w->fillRectangle(25 * x + 125 + 1, 25 * y + 125 + 1, 23, 23, c);
}

void Cell::undraw(Xwindow* w, int x, int y) {
	w->fillRectangle(25 * x + 125 + 1, 25 * y + 125 + 1, 23, 23, 0);
}