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

	soundClip.loadFromFile("../Audio/bulletFire.ogg");
	soundPlayer.play();
}


Bullet::~Bullet()
{
	liveBullet = false;
}


void Bullet::update(CentipedeGame*)
{
	//update bullet pos every tick based on velocity.
	if (CentipedeGame::clock % delay == 0)
		currentPosition.y += velocity.y;
	currentPosition.x = xStart;
}


//Called in CentipedeGame.
void Bullet::collideWith(GameObject * other)
{
	health = 0;
}


//Overrides GameObject's die and adds instance variables affected.
unsigned int Bullet::die(bool &readyToDie, CentipedeGame *gameHandle) {
	liveBullet = false;
	return GameObject::die(readyToDie, gameHandle);
}