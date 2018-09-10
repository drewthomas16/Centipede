#pragma once
#include "GameObject.h"

//TODO: Make centipede work without mushroom spawning on death.

class CentipedeSegment : public GameObject
{
public:
	CentipedeSegment();
	CentipedeSegment(int, int);
	~CentipedeSegment();

	void attach(CentipedeSegment*);

	void update(CentipedeGame *gameHandle);
	void collideWith(GameObject *);

	void setAsHead();

	void calculateVelocity();

	char* getType() { return "CentipedeSegment"; }

	unsigned int die(bool&, CentipedeGame*);

private:
	bool isPoisoned;
	bool movingDown, movingRight;
	bool deathCollisionDanger;
	CentipedeSegment * previous = nullptr;
};