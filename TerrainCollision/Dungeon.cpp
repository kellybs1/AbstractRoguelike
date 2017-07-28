#include "Dungeon.h"
#include "TileMap.h"

/*
Dungeon class: Creates, contains and manages a dungeon built of Rooms and Cell.
Developed by Brendan Kelly throughout September, 2016.
*/

Dungeon::Dungeon(Random^ inRand)
{
	nRooms = 1;
	rand = inRand;
	wholeDungeon = gcnew array<Cell^, 2>(ROWCOUNT, COLCOUNT);
	individualRooms = gcnew array<Room^>(nRooms);
}

void Dungeon::RandomRoomCount()
{
	nRooms = rand->Next(ROOM_COUNT_MIN, ROOM_COUNT_MAX);
	individualRooms = gcnew array<Room^>(nRooms);
}

//encapsulating method for generating an entirely new dungeon
void Dungeon::GenerateNewDungeon()
{
	clearDungeon();

	for (int r = 0; r < nRooms; r++)
	{
		makeRoom(r);
	}

	for (int r = 0; r < nRooms - 1; r++) //stop at second last index because last corridor made goes from second last to last room
	{
		makeCorridor(r, r + 1);
	}
}

//generates a room
void Dungeon::makeRoom(int index)
{
	//mins and max to leave one tile always non walkable around the edge
	int roomColMin = 1;
	int roomRowMin = 1;
	int roomColMax = COLCOUNT - 2;
	int roomRowMax = ROWCOUNT - 2;
	//generate a room, randomise size and location
	Room^ testRoom;
	bool roomOK = false;
	while (roomOK == false)
	{
		int testWidth = rand->Next(ROOM_SIZE_MIN, ROOM_SIZE_MAX);
		int testHeight = rand->Next(ROOM_SIZE_MIN, ROOM_SIZE_MAX);
		int testX = rand->Next(roomColMin, roomColMax-testWidth);
		int testY = rand->Next(roomRowMin, roomRowMax-testHeight);
		testRoom = gcnew Room(testWidth, testHeight, testX, testY);
		//if it's the first one, just use it
		if (index == 0)
		{
			roomOK = true;
		}
		else
		{
			for (int i = 0; i < index; i++)
			{
				//if this room shares space with another, start over
				if (roomCollision(testRoom) == false) 
				{
					roomOK = true;
				}
				else
				{
					roomOK = false;
				}
			}
		}
	}
	individualRooms[index] = testRoom;
	putRoomInDungeon(testRoom);
	
}

//checks if any of the cells are already in use so a new room can't be put here
bool Dungeon::roomCollision(Room^ testRoom)
{
	//variabilise
	int leftCol = testRoom->LeftCol;
	int topRow = testRoom->TopRow;
	int nCols = testRoom->Width;
	int nRows = testRoom->Height;

	//comparisons
	bool collided = false;

	for (int r = topRow; r < topRow+nRows; r++)
	{
		for (int c = leftCol; c < leftCol+nCols; c++)
		{
			if (wholeDungeon[r, c]->CellType != CELL_TYPE_DIRT)
			{
				collided = true;
			}
		}
	}

	return collided;
}


//inserts a generated room into the dungeon map
void Dungeon::putRoomInDungeon(Room^ newRoom)
{
	//set all the walls as walltype
	int roomLeftCol = newRoom->LeftCol;
	int roomHeight = newRoom->Height;
	int roomTopRow = newRoom->TopRow;
	int roomWidth = newRoom->Width;
	for (int r = roomTopRow; r < roomTopRow + roomHeight; r++)
	{
		for (int c = roomLeftCol; c < roomLeftCol + roomWidth; c++)
		{
			//set the borders as walls
			if (r == roomTopRow || c == roomLeftCol)
			{
				//left and top
				wholeDungeon[r, c]->CellType = CELL_TYPE_WALL;
			}
			else
			{
				if (r == roomTopRow + roomHeight - 1 || c == roomLeftCol + roomWidth - 1)
				{
					//right and bottom
					wholeDungeon[r, c]->CellType = CELL_TYPE_WALL;
				}
				else
				{
					//so everything else in the room must be floor
					wholeDungeon[r, c]->CellType = CELL_TYPE_FLOOR;
				}
			}
		}
	}
	
}

//build a corridor between two rooms
void Dungeon::makeCorridor(int room1, int room2)
{
	//variabialise
	Room^ r1 = individualRooms[room1];
	Room^ r2 = individualRooms[room2];
	int xCheckR1 = r1->LeftCol + (r1->Width / 2);
	int xCheckR2 = r2->LeftCol + (r2->Width / 2);
	//if r2 is west of r1 swap them
	if (xCheckR2 < xCheckR1)
	{
		Room^ temp = r1;
		r1 = r2;
		r2 = temp;
	}

	int centreR1Col = r1->LeftCol + (r1->Width / 2);
	int centreR1Row = r1->TopRow + (r1->Height / 2);
	int centreR2Col = r2->LeftCol + (r2->Width / 2);
	int centreR2Row = r2->TopRow + (r2->Height / 2);
	//WEST -> EAST
	for (int i = centreR1Col; i < centreR2Col; i++)
	{
		Cell^ cellCheck = wholeDungeon[centreR1Row, i];
		switch (cellCheck->CellType)
		{
		case CELL_TYPE_WALL:
			wholeDungeon[centreR1Row, i]->CellType = CELL_TYPE_DOOR;
			break;

		case CELL_TYPE_FLOOR:
			wholeDungeon[centreR1Row, i]->CellType = CELL_TYPE_FLOOR;
			break;

		case CELL_TYPE_DOOR:
			wholeDungeon[centreR1Row, i]->CellType = CELL_TYPE_DOOR;
			break;

		default:
			wholeDungeon[centreR1Row, i]->CellType = CELL_TYPE_CORRIDOR;
			break;
		}

	}

	//NORTHSOUTH
	int startYCorrRow = 0;
	int endYCorrRow = 0;
	if (centreR2Row < centreR1Row)
	{
		startYCorrRow = centreR2Row;
		endYCorrRow = centreR1Row + 1; // the plus one here stops dead ended corners
	}
	else
	{
		startYCorrRow = centreR1Row;
		endYCorrRow = centreR2Row;
	}
	for (int i = startYCorrRow; i < endYCorrRow; i++)
	{
		Cell^ cellCheck = wholeDungeon[i, centreR2Col];
		switch (cellCheck->CellType)
		{
		case CELL_TYPE_WALL:
			wholeDungeon[i, centreR2Col]->CellType = CELL_TYPE_DOOR;
			break;

		case CELL_TYPE_FLOOR:
			wholeDungeon[i, centreR2Col]->CellType = CELL_TYPE_FLOOR;
			break;

		case CELL_TYPE_DOOR:
			wholeDungeon[i, centreR2Col]->CellType = CELL_TYPE_DOOR;
			break;

		default:
			wholeDungeon[i, centreR2Col]->CellType = CELL_TYPE_CORRIDOR;
			break;
		}
	}

	
}


//wipes the dungeon clear in the case a new dungeon is to be generated
void Dungeon::clearDungeon()
{
	for (int r = 0; r < ROWCOUNT; r++)
	{
		for (int c = 0; c < COLCOUNT; c++)
		{
			wholeDungeon[r, c] = gcnew Cell(r, c, CELL_TYPE_DIRT);
		}
	}
}

//converts dungeon to a form used by the TileMap class
array<int, 2>^ Dungeon::GenerateTileMapFromDungeon()
{
	array<int, 2>^ outArray = gcnew array<int, 2>(ROWCOUNT, COLCOUNT);
	//assign each array member a corresponding tile type to the types of tiles used for drawing
	for (int r = 0; r < ROWCOUNT; r++)
	{
		for (int c = 0; c < COLCOUNT; c++)
		{
			int workingCellType = wholeDungeon[r, c]->CellType;
			switch (workingCellType)
			{
			case CELL_TYPE_DIRT:
				outArray[r, c] = TILE_TYPE_DIRT;
				break;

			case CELL_TYPE_WALL:
				outArray[r, c] = TILE_TYPE_WALL;
				break;

			case CELL_TYPE_FLOOR:
				outArray[r, c] = TILE_TYPE_FLOOR;
				break;

			case CELL_TYPE_CORRIDOR:
				outArray[r, c] = TILE_TYPE_CORRIDOR;
				break;

			case CELL_TYPE_DOOR:
				outArray[r, c] = TILE_TYPE_DOOR;
				break;
			}
		}
	}
	return outArray;
}
