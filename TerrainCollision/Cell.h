#pragma once

/*
Cell class: Contains a single cell used by a Room.
Developed by Brendan Kelly throughout September, 2016.
*/

#define CELL_TYPE_DIRT 0
#define CELL_TYPE_WALL 1
#define CELL_TYPE_FLOOR 2
#define CELL_TYPE_CORRIDOR 3
#define CELL_TYPE_DOOR 4


ref class Cell
{
public:
	Cell(int inRow, int inCol, int inCellType);

	property int Col;
	property int Row;
	property int CellType;
};

