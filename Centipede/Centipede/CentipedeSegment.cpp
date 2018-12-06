#include "stdafx.h"
#include "CentipedeSegment.h"
#include "CentipedeGame.h"
#include "Mushroom.h"
#include "Player.h"
#include <math.h>

CentipedeSegment::CentipedeSegment()
{
	//What am I doing??!?!
}


//Starts a centipede in the game.
CentipedeSegment::CentipedeSegment(int x, int y) : GameObject(x, y)
{
	pointValue = 10;
	health = 1;
	isPoisoned = false;

	setTexture("../Sprites/CentipedeSegment/default.png");
	object.setOrigin(1, 1);

	velocity = sf::Vector2f(1, 0);

	movingDown = movingRight = true;

	objectsPtr = nullptr;
}


CentipedeSegment::~CentipedeSegment()
{
}


//Remeber the last centipede segment.
void CentipedeSegment::attach(CentipedeSegment *segment)
{
	previous = segment;
}


//Change the position of the centipede by  calculating the velocity.
//Make sure that you do not check this everytime to give it a pause.
void CentipedeSegment::update(CentipedeGame *gameHandle)
{

	if (CentipedeGame::clock % 2 == 0) 
	{

		calculateVelocity();

		//update position
		currentPosition.x += velocity.x;
		currentPosition.y += velocity.y;

		//update velocity for next movement
	}

}


//Kill this segement of the centipede if it collides with a Bullet.
void CentipedeSegment::collideWith(GameObject * other)
{
	deathCollisionDanger = false;
	if (dynamic_cast<Player *>(other) != nullptr)
	{
		--health;
		deathCollisionDanger = true;
	}	
	else if (dynamic_cast<Bullet *>(other) != nullptr)
		--health;
}


//Kill the centipede segment and spawn a mushroom in its place.
unsigned int CentipedeSegment::die(bool &readyToDie, CentipedeGame *gameHandle) 
{
	
	if(!deathCollisionDanger)
		gameHandle->spawnObject<Mushroom>(currentPosition.x, currentPosition.y);

	readyToDie = true;

	if (previous != nullptr)
		previous->setAsHead();

	return pointValue;
}


//If the segement infront was the head and it dies become the head.
void CentipedeSegment::setAsHead() {
	setTexture("../Sprites/CentipedeSegment/head.png");
}

//Calculate the velocity of the sentipede to use in update.
void CentipedeSegment::calculateVelocity() 
{
	if (velocity.y != 0)
	{ //i changed rows

		if (velocity.y > 0)
			movingDown = true;
		else
			movingDown = false;

		velocity.y = 0; //velocity.x and velocity.y are mutually exclusive

		velocity.x = pow(-1, movingRight);
		movingRight = !movingRight; //flip x directions
	}

	if (!canMoveTo(currentPosition.x + velocity.x, currentPosition.y + velocity.y))
	{
		if (currentPosition.y == 0) { //top
			velocity.y = 1; // will cause movingDown to be true next cycle
		}
		else if (currentPosition.y == 29) { //bottom
			velocity.y = -1; //will cause movingDown to be false next cycle
		}
		else { //side or mushroom
			velocity.y = pow(-1, !movingDown);
		}

		velocity.x = 0; //velocity.x and velocity.y are mutually exclusive
	}
}

//Make sure that a centipede can move somewhere by checking to make sure there
//is no mushroom there and it is not off the screen.
bool CentipedeSegment::canMoveTo(double x, double y)
{
	if (!(x < 30 && x >= 0 && y < 30 && y >= 0))
		return false;
	if (objectsPtr != nullptr) {
		for (int i = 0; i < (objectsPtr + 3)->size(); ++i)
			if ((objectsPtr + 3)->at(i)->getSprite()->
				getGlobalBounds().contains(x * interval.x, y * interval.y))
				return false;
	}
	return true;
}

void CentipedeSegment::setObjectsPtr(std::vector<std::shared_ptr<GameObject>>* entitylist)
{
	objectsPtr = entitylist;
}
