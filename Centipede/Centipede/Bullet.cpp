#include "stdafx.h"
#include "Bullet.h"
#include "CentipedeGame.h"
#include "Mushroom.h"

bool Bullet::liveBullet = false;


Bullet::Bullet(double x, double y) : GameObject(x, y)
{
	xStart = x;
	health = 1;
	setVelocity(sf::Vector2f(0, -1));
	setTexture("../Sprites/bullet.png");
	object.setOrigin(-4, -4);
	liveBullet = true;
	pointValue = 0;
	currentPosition.x = xStart;
}


Bullet::~Bullet()
{
	liveBullet = false;
}


void Bullet::update(CentipedeGame* gameHandle)
{
	//update bullet pos every tick based on velocity.
	if (CentipedeGame::clock % delay == 0)
		if(gameHandle->isInBounds(currentPosition.x, currentPosition.y + velocity.y))
			currentPosition.y += velocity.y;
		else
			health = 0;
}


//Called in CentipedeGame.
void Bullet::collideWith(GameObject * other)
{
	health = 0;
	std::cout << "help" << std::endl;
	other->collideWith(this);
}


//Overrides GameObject's die and adds instance variables affected.
unsigned int Bullet::die(bool &readyToDie, CentipedeGame *gameHandle) {
	liveBullet = false;
	return GameObject::die(readyToDie, gameHandle);
}