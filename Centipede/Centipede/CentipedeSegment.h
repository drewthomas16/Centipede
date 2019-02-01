#pragma once
#include "GameObject.h"


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

	bool canMoveTo(double x, double y1, double y2);

	void setObjectsPtr(std::vector<std::shared_ptr<GameObject>>* entitylist);

	char* getType() { return "CentipedeSegment"; }

	unsigned int die(bool&, CentipedeGame*);

	void newSprite(int & i, int whichWayMoving);

	double getSpeed();
	void setSpeed(double i);

private:
	bool isPoisoned;
	double speed;
	//variables for whichWayMoving are as follows. 1 - moving right, 2 - moving left,
	//3 - moving down, 4 - moving up
	int whichWayMoving, spriteNum;
	bool movingDown, movingRight, head;
	bool deathCollisionDanger;
	CentipedeSegment * previous = nullptr;
	std::vector<std::shared_ptr<GameObject>> * objectsPtr;
};