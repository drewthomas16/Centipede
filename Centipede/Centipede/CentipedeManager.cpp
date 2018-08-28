#include "CentipedeManager.h"
#include "CentipedeSegment.h"
#include <iostream>
#include "CentipedeGame.h"

//Used to prevent memory errors.
CentipedeManager::CentipedeManager() {

	gameHandle = nullptr;
}

//Bind game manager to only affect current game.
void CentipedeManager::bindToGame(CentipedeGame *handle) {
	gameHandle = handle;
}

//Used in CentipedeGame to determine if a mushroom should spawn in
//a given spot.
void CentipedeManager::calculateEntryX() {
	do {
		entryX = rand() % 30;
	} while (CentipedeGame::isMushroomCell(entryX, 0));
}

//sets spawn settings at beginning of game.
bool CentipedeManager::beginSpawn(unsigned int frame, unsigned int _speed, unsigned int _length) {

	bool status;

	//Done is true by default.
	if (done) {

		done = false;

		calculateEntryX();
		entryTime = frame;
		speed = _speed;
		haveSpawned = 0;
		length = _length;

		status = true;
	}
	else
		status = false;

	return status;
}

void CentipedeManager::update() {
	if (!done && (CentipedeGame::clock % speed == 0)) {
		segments.push_back(gameHandle->spawnObject<CentipedeSegment>(entryX, 0));
		if (++haveSpawned >= length)
			done = true;
	}
}

void CentipedeManager::placeSegment() {
}