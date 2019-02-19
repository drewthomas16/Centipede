#include "CentipedeManager.h"
#include "CentipedeSegment.h"
#include "CentipedeGame.h"
#include <iostream>

//Used to prevent memory errors.
CentipedeManager::CentipedeManager() 
{

	gameHandle = nullptr;
	end = -1;
}

//Bind game manager to only affect current game.
void CentipedeManager::bindToGame(CentipedeGame *handle) 
{
	gameHandle = handle;
}

//Used in CentipedeGame to determine if a mushroom should spawn in
//a given spot.
int CentipedeManager::calculateEntryX() 
{
	int x;
	do {
		x = rand() % 30;
	} while (CentipedeGame::isMushroomCell(x, 0));
	return x;
}

//sets spawn settings at beginning of game.
bool CentipedeManager::beginSpawn(unsigned int frame,double _speed, unsigned int _length) 
{

 	end++;
	entryX.push_back(calculateEntryX());
	entryTime.push_back(frame);
	speed.push_back(_speed);
	haveSpawned.push_back(0);
	length.push_back(_length);

	return true;
}

void CentipedeManager::update() 
{
	if (end > -1 && speed.size() > 0) 
	{
		/*
		This decides how often to spawn centipede segments.
		This is calculated based on how long it takes for a given segment
		to travel 8 pixels or the width of a segment. Represented by 8/(the speed of a segment
		given by pixels traveled per cycle).
		*/
		if (CentipedeGame::clock % (int)(((8. * 2.) / 7.) / speed.at(end)) == 0)
		{
			gameHandle->spawnObject<CentipedeSegment>(entryX.at(end), 0)->setSpeed(speed.at(end));
			if (haveSpawned.at(end) + 1 >= length.at(end))
				end--;
			else
				haveSpawned.at(end)++;
		}
	}
}


void CentipedeManager::clear()
{
	entryX.clear();
	entryTime.clear();
	speed.clear();
	haveSpawned.clear();
	length.clear();
	end = -1;

}

int CentipedeManager::getEnd()
{
	return end;
}