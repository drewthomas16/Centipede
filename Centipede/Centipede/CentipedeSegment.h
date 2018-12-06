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

	bool canMoveTo(double x, double y);

	void setObjectsPtr(std::vector<std::shared_ptr<GameObject>>* entitylist);

	char* getType() { return "CentipedeSegment"; }

	unsigned int die(bool&, CentipedeGame*);

	double getSpeed();
	void setSpeed(double i);

private:
	bool isPoisoned;
	double speed;
	bool movingDown, movingRight;
	bool deathCollisionDanger;
	CentipedeSegment * previous = nullptr;
	std::vector<std::shared_ptr<GameObject>> * objectsPtr;
};