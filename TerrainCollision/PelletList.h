#pragma once

//
//PelletList manages a dynamic list of Pellet objects
//

#include "Pellet.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

ref class PelletList
{
private:
	Pellet^ head;
	Pellet^ tail;

public:
	PelletList(void);
	void addPellet(Pellet^ newPellet);
	void deleteOnePellet(Pellet^ pelletToDelete);
	void deleteAllDeadPellets();
	void movePellets();
	void drawPellets();
	void hitWallAllPellets();
	int countPellets();
};
