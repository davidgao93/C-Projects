/*
* Cell.h
* Cell.h is the header file for the Cell class
* A cell represents one unit block on a grid.
*/

#ifndef __CELL_H__
#define __CELL_H__
#include "window.h"

class Cell {
  public:
	char type;
	int lv;
	int blockID;

	// Constructors
	Cell();
	Cell(char, int, int);
	
	// Destructor
	~Cell();

	void draw(Xwindow*, int, int, int);
	void undraw(Xwindow*, int, int);
};
#endif