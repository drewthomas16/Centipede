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

	velocity.y = 0;
	speed = 1;
	velocity.x = speed;
	whichWayMoving = 1;
	spriteNum = 0;
	newSprite(spriteNum, whichWayMoving);

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
	head = false;
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

		if (velocity.x > 0)
			whichWayMoving = 1;
		else if (velocity.x < 0)
			whichWayMoving = 2;
		else if (velocity.y > 0)
			whichWayMoving = 3;
		else if (velocity.y < 0)
			whichWayMoving = 4;

		//update velocity for next movement
	}
	//std::cout << previous << std::endl;
	if (previous == nullptr)
		setAsHead();
	//We update sprite every time so that spites don't just update with movement.
	newSprite(spriteNum, whichWayMoving);

	

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

	if (!deathCollisionDanger)
		gameHandle->spawnObject<Mushroom>(currentPosition.x, currentPosition.y);

	readyToDie = true;

	if (previous != nullptr)
		previous->setAsHead();

	return pointValue;
}


//If the segement infront was the head and it dies become the head.
void CentipedeSegment::setAsHead() 
{
	head = true;
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

		//velocity.x = pow(-1, movingRight);
		if (movingRight)
			velocity.x = -1 * speed;
		else
			velocity.x = speed;
		movingRight = !movingRight; //flip x directions
	}

	if (!canMoveTo(currentPosition.x + velocity.x,
		currentPosition.y + speed,
		currentPosition.y + (-1 * speed)))
	{
		if (currentPosition.y + velocity.y < 0) 
		{ //top
			velocity.y = speed; // will cause movingDown to be true next cycle
		}
		else if (currentPosition.y + velocity.y > 28) 
		{ //bottom
			velocity.y = -1 * speed; //will cause movingDown to be false next cycle
		}
		else 
		{ //side of mushroom
			if (movingDown)
			{
				velocity.y = speed;
			}
			else
			{
				velocity.y = -1 * speed;
			}
		}

		velocity.x = 0; //velocity.x and velocity.y are mutually exclusive
	}
	else
	{
		if (movingRight)
			whichWayMoving = 0;
		else
			whichWayMoving = 1;
	}
}

//Make sure that a centipede can move somewhere by checking to make sure there
//is no mushroom there and it is not off the screen.
bool CentipedeSegment::canMoveTo(double x, double y1, double y2)
{
	//keep within the boundries of the screen.
	if (x > 29 || x < 0 || y1 >= 30 || y2 <= 0)
		return false;


	//Inital FloatRect values based off of position.
	double boxLeft = object.getGlobalBounds().left;
	double boxTop = object.getGlobalBounds().top;
	double boxWidth = object.getGlobalBounds().width;
	double boxHeight = object.getGlobalBounds().height;

	double adjustVeloX = velocity.x, adjustVeloY = velocity.y;

	//Pad the velocity values.
	if (adjustVeloX < 0)
		adjustVeloX = -3;
	else if (adjustVeloX > 0)
		adjustVeloX = 3;
	else
		adjustVeloX = 0;

	//Create FloatRect.
	sf::FloatRect futurePosRect(boxLeft + adjustVeloX, boxTop,
		boxWidth + adjustVeloX, boxHeight);

	//Check for collisions.
	if (objectsPtr != nullptr) {
		for (int i = 0; i < (objectsPtr + 3)->size(); ++i)
		{
			//Dynamic cast raw pointer to a mushroom so the compiler stays quiet.
			Mushroom * touchedMushroom = dynamic_cast<Mushroom*>((objectsPtr + 3)->at(i).get());
			if ((objectsPtr + 3)/*mushroom*/->at(i)->getSprite()->
				getGlobalBounds().intersects(futurePosRect)
				&& touchedMushroom != nullptr
				//We don't want to collide with pushed mushrooms.
				&& !touchedMushroom->isPushed())
			{
				//std::cout << "TEST " << std::endl;

				//Centipede has to 'touch' poisoned mushrooms so that they know to fall.
				touchedMushroom->push();
				if(!touchedMushroom->getPoisoned())
					return false;
			}
		}
	}
	return true;
}

//variables for whichWayMoving are as follows. 1 - moving right, 2 - moving left,
//3 - moving down, 4 - moving up
void CentipedeSegment::newSprite(int & i, int whichWayMoving)
{
	std::string filePath;
	switch (whichWayMoving)
	{
	case 1:
		if (!head)
		{
			if (i < 7)
				i++;
			else
				i = 0;
			filePath = "../Sprites/HorizontalBody/" + std::to_string(i) + ".png";
			setTexture(filePath.c_str());
			if (object.getScale().x != -1)
				object.setScale(-1, 1);
		}
		else
		{
			if (i < 7)
				i++;
			else
				i = 0;
			filePath = "../Sprites/HorizontalHead/" + std::to_string(i) + ".png";
			setTexture(filePath.c_str());
			if (object.getScale().x != -1)
				object.setScale(-1, 1);
		}
		break;
	case 2:
		if (!head)
		{
			if (i > 0)
				i--;
			else
				i = 7;
			filePath = "../Sprites/HorizontalBody/" + std::to_string(i) + ".png";
			setTexture(filePath.c_str());
			if (object.getScale().x != 1)
				object.setScale(1, 1);
		}
		else
		{
			if (i > 0)
				i--;
			else
				i = 7;
			filePath = "../Sprites/HorizontalHead/" + std::to_string(i) + ".png";
			setTexture(filePath.c_str());
			if (object.getScale().x != 1)
				object.setScale(1, 1);
		}
		break;
	case 3:
		if (!head)
		{
			if (i < 5)
				i++;
			else
				i = 0;
			filePath = "../Sprites/VerticalBody/" + std::to_string(i) + ".png";
			setTexture(filePath.c_str());
			if (object.getScale().y != 1)
				object.setScale(1, 1);
		}
		else
		{
			if (i < 5)
				i++;
			else
				i = 0;
			filePath = "../Sprites/VerticalHead/" + std::to_string(i) + ".png";
			setTexture(filePath.c_str());
			if (object.getScale().y != 1)
				object.setScale(1, 1);
		}
		break;
	case 4:
		if (!head)
		{
			if (i > 0 && i <= 5)
				i--;
			else
				i = 5;
			filePath = "../Sprites/VerticalBody/" + std::to_string(i) + ".png";
			setTexture(filePath.c_str());
			if (object.getScale().y != -1)
				object.setScale(1, -1);
		}
		else
		{
			if (i > 0 && i <= 5)
				i--;
			else
				i = 5;
			filePath = "../Sprites/VerticalHead/" + std::to_string(i) + ".png";
			setTexture(filePath.c_str());
			if (object.getScale().y != -1)
				object.setScale(1, -1);
		}
		break;
	}
//setTexture("../Sprites/CentipedeSegment/default.png");
}

//Needs to be fed the pointer that holds entities for collision.
void CentipedeSegment::setObjectsPtr(std::vector<std::shared_ptr<GameObject>>* entitylist)
{
	objectsPtr = entitylist;
}


double CentipedeSegment::getSpeed()
{
	return speed;
}

void CentipedeSegment::setSpeed(double i)
{
	speed = i;
	velocity.x = speed;
}