#pragma once
#include "Room.h"

/*
Dungeon class: Creates, contains and manages a dungeon built of Rooms and Cell.
Developed by Brendan Kelly throughout September, 2016.
*/

#define ROOM_COUNT_MIN 4
#define ROOM_COUNT_MAX 8

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

//Contains a dungeon built of room and generates a map used by TileMap

ref class Dungeon
{
private:
	array<Cell^, 2>^ wholeDungeon;
	array<Room^>^ individualRooms;
	int nRooms;
	Random^ rand;
	void putRoomInDungeon(Room^ testRoom);	
	bool roomCollision(Room^ testRoom);
	void makeCorridor(int room1, int room2);
	void makeRoom(int index);
	void clearDungeon();


public:
	Dungeon(Random^ inRand);
	void GenerateNewDungeon();		
	array<int, 2>^ GenerateTileMapFromDungeon();
	void RandomRoomCount();

};

