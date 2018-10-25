#pragma once

#include <vector>
#include <memory>

class CentipedeGame;
class CentipedeSegment;

class CentipedeManager {
public:
	CentipedeManager();

	void bindToGame(CentipedeGame*);

	int calculateEntryX();

	bool beginSpawn(unsigned int, unsigned int, unsigned int);

	void update();
	void clear();

private:
	CentipedeGame *gameHandle;
	void placeSegment();

	std::vector<int> entryTime, haveSpawned, entryX, speed, length;
	int end;
	bool done = true;
	std::vector<std::shared_ptr<CentipedeSegment>> segments;
};