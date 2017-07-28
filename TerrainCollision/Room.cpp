#include "Room.h"

/*
Room class: contains a room built of Cells used by the Dungeon
Developed by Brendan Kelly throughout September, 2016.
*/

Room::Room(int inWidth, int inHeight, int inLeftCol, int inTopRow)
{
	Width = inWidth;
	Height = inHeight;
	LeftCol = inLeftCol;
	TopRow = inTopRow;
}

