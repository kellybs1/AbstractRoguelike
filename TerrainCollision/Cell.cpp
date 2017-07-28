#include "Cell.h"

/*
Cell class: Contains a single cell used by a Room.
Developed by Brendan Kelly throughout September, 2016.
*/

Cell::Cell(int inRow, int inCol, int inCellType)
{
	Col = inCol;
	Row = inRow;
	CellType = inCellType;
}
