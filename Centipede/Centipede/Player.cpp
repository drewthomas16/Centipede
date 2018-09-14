#include "stdafx.h"
#include "Player.h"
#include "CentipedeGame.h"
#include "CentipedeSegment.h"
#include "Scorpion.h"
#include "Spider.h"
#include <math.h>

Player::Player(int x, int y) : GameObject(x, y)
{
	setTexture("../Sprites/player.png");
	health = 3;
	shotBullet = false;
}


void Player::update(CentipedeGame *gameHandle)
{
	velocity.x = 0;
	velocity.y = 0;

	//Up and down movement.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			velocity.y = movementSpeed / sqrt(2) * -1;
		else
			velocity.y = movementSpeed * -1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			velocity.y = movementSpeed / sqrt(2);
		else
			velocity.y = movementSpeed;

	//Left and right movement.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			velocity.x = movementSpeed / sqrt(2) * -1;
		else
			velocity.x = movementSpeed * -1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			velocity.x = movementSpeed / sqrt(2);
		else
			velocity.x = movementSpeed;

	//If space is pressed make a bullet in the Player's location.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !shotBullet)
	{
		gameHandle->spawnObject<Bullet>(currentPosition.x, currentPosition.y);
		shotBullet = true;

		//SoundClip has to be here.
		soundClip.loadFromFile("../Audio/bulletFire.ogg");
		soundPlayer.play();
	}
	else if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		shotBullet = false;
	
	if (!canMoveTo(currentPosition.x + velocity.x, currentPosition.y + velocity.y))
	{
		velocity.x = 0;
		velocity.y = 0;
	}

	//Move the player based off the velocity.
	//Only allow the play to move every eight frames to a stuttered look.

	std::vector<sf::FloatRect> allOfTheBounds;

	for (int i = 0; i < thingPtr->size(); ++i)
		allOfTheBounds.push_back(*thingPtr->at(i)->getGlobalBounds());
	
	//Create a new instance of sprite adjusted for velocity and get bounding boxes.
	//if the new bounding box collides with any of allOfTheBound's floatrects dont go forward.

	if (gameHandle->isInBounds(currentPosition.x + velocity.x, currentPosition.y))
			currentPosition.x += velocity.x;
	if (gameHandle->isInBounds(currentPosition.x, currentPosition.y + velocity.y))
			currentPosition.y += velocity.y;
}

void Player::collideWith(GameObject * other)
{
	if (dynamic_cast<CentipedeSegment *>(other) != nullptr)
		health--;
	else if (dynamic_cast<Flea *>(other) != nullptr)
		health--;
	else if (dynamic_cast<Scorpion *>(other) != nullptr)
		health--;
	else if (dynamic_cast<Spider *>(other) != nullptr)
		health--;

	//play a death anim here
}

void Player::setThingPtr(std::vector<std::shared_ptr<GameObject>>* gameList)
{
	thingPtr = gameList;
}


/*Old player method.

setVelocity(
		sf::Vector2i(getNearestCellPos(gameHandle->getRelMousePos()).x - currentPosition.x,
		getNearestCellPos(gameHandle->getRelMousePos()).y-currentPosition.y));

	currentPosition = getNearestCellPos(gameHandle->getRelMousePos());
	//currentPosition /= static_cast<int>(interval.x);
	//if (!mushroom in way)
	//goto mouse
	//else

	// go untill hit mushroom
	/*
	if ((30 - currentPosition.y) > 12)
	currentPosition.y = 30-12;
	if (currentPosition.y > 29)
	currentPosition.y = 29;
	if (currentPosition.x < 0)
	currentPosition.x = 0;
	if (currentPosition.x > 29)
	currentPosition.x = 29;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !Bullet::liveBullet)
	{
		//gameHandle->spawnObject<Bullet>(currentPosition.x, currentPosition.y);
	}*/
	//object.setPosition(gameHandle->getRelMousePos().x, gameHandle->getRelMousePos().y);