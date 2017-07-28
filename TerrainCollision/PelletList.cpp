#include "PelletList.h"

//
//PelletList manages a dynamic list of Pellet objects
//

PelletList::PelletList()
{
}

//adds a new pellet
void PelletList::addPellet(Pellet^ newPellet)
{
	if (head == nullptr)
	{
		tail = newPellet;
		head = newPellet;
	}
	else
	{
		tail->Next = newPellet;
		tail = newPellet;
	}
}

//deletes one pellets 
void PelletList::deleteOnePellet(Pellet^ pelletToDelete)
{
	if (head == pelletToDelete) //if you are deleting the first node
	{
		if (tail == pelletToDelete) //if it is the only node in the list
		{
			head = nullptr; //head and tail both become nullptr
			tail = nullptr;
		}
		else
		{
			head = pelletToDelete->Next; //move head along to the second node
		}
	}
	else
	{
		Pellet^ nodeWalker = head;
		while (nodeWalker->Next != pelletToDelete) //find the node prior to the one to be deleted
		{
			nodeWalker = nodeWalker->Next;
		}
		nodeWalker->Next = pelletToDelete->Next; //change its next pointer to the deleted node's next pointer
		if (nodeWalker->Next == nullptr) //if you just deleted the last node in the list
		{
			tail = nodeWalker; //set the tail to the prior node
		}
	}
}

//deletes all dead pellets
void PelletList::deleteAllDeadPellets()
{
	Pellet^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		if (nodeWalker->IsAlive == false)
		{
			deleteOnePellet(nodeWalker);
		}
		nodeWalker = nodeWalker->Next;
	}
}

//moves all pellets
void PelletList::movePellets()
{
	Pellet^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		nodeWalker->move();
		nodeWalker = nodeWalker->Next;
	}
}

//draws all pellets
void PelletList::drawPellets()
{
	Pellet^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		nodeWalker->draw();
		nodeWalker = nodeWalker->Next;
	}
}

//returns a count of total pellets
int PelletList::countPellets()
{
	Pellet^ nodeWalker = head;
	int count = 0;

	while (nodeWalker != nullptr)
	{
		count++;
		nodeWalker = nodeWalker->Next;
	}
	return count;
}

//kills all pellets that have hit walls
void PelletList::hitWallAllPellets()
{
	Pellet^ nodeWalker = head;
	while (nodeWalker != nullptr)
	{
		nodeWalker->hitWall();
		nodeWalker = nodeWalker->Next;
	}

}
