#include "CentipedeManager.h"
#include "CentipedeSegment.h"
#include <iostream>
#include "CentipedeGame.h"

//Used to prevent memory errors.
CentipedeManager::CentipedeManager() {

	gameHandle = nullptr;
	end = -1;
}

//Bind game manager to only affect current game.
void CentipedeManager::bindToGame(CentipedeGame *handle) {
	gameHandle = handle;
}

//Used in CentipedeGame to determine if a mushroom should spawn in
//a given spot.
int CentipedeManager::calculateEntryX() {
	int x;
	do {
		x = rand() % 30;
	} while (CentipedeGame::isMushroomCell(x, 0));
	return x;
}

//sets spawn settings at beginning of game.
bool CentipedeManager::beginSpawn(unsigned int frame, unsigned int _speed, unsigned int _length) {

	bool status;

	//Done is true by default.
	/*if (done) {

		done = false;

		calculateEntryX();
		entryTime = frame;
		speed = _speed;
		haveSpawned = 0;
		length = _length;

		status = true;
	}
	else
		status = false;*/
	end++;
	entryX.push_back(calculateEntryX());
	entryTime.push_back(frame);
	speed.push_back(_speed);
	haveSpawned.push_back(0);
	length.push_back(_length);

	return true;
}

void CentipedeManager::update() {
	if (end > -1 && CentipedeGame::clock % speed.at(end) == 0) 
	{
		segments.push_back(gameHandle->spawnObject<CentipedeSegment>(entryX.at(end), 0));
		if (haveSpawned.at(end) + 1 >= length.at(end))
			end--;
		else
			haveSpawned.at(end) += 1;
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

void CentipedeManager::placeSegment() {
}