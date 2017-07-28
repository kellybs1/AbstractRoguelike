#pragma once
#include "Cell.h"

/*
Room class: contains a room built of Cells used by the Dungeon
Developed by Brendan Kelly throughout September, 2016.
*/

#define ROOM_SIZE_MIN 6
#define ROOM_SIZE_MAX 15

ref class Room
{
private:

public:
	Room(int inWidth, int inHeight, int inLeftCol, int inTopRow);
	property int Width;
	property int Height;
	property int LeftCol;
	property  int TopRow;


};

