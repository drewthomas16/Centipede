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

	bool beginSpawn(unsigned int, double, unsigned int);

	void update();
	void clear();
	int getEnd();

private:
	CentipedeGame *gameHandle;


	std::vector<int> entryTime, haveSpawned, entryX, length;
	std::vector<double> speed;
	int numSpawned;
	int end;
	bool done = true;
	std::vector<std::shared_ptr<CentipedeSegment>> segments;
};