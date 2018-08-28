#include "stdafx.h"
#include "Player.h"
#include "CentipedeGame.h"
#include "CentipedeSegment.h"
#include "Scorpion.h"
#include "Spider.h"

Player::Player(int x, int y) : GameObject(x, y)
{
	setTexture("../Sprites/player.png");
	health = 3;
}


void Player::update(CentipedeGame *gameHandle)
{
	velocity.x = 0;
	velocity.y = 0;

	//Up and down movement.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		velocity.y = -1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		velocity.y = 1;
	
	//Left and right movement.
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		velocity.x = -1;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		velocity.x = 1;

	//If space is pressed make a bullet in the Player's location.
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) 
		//&& CentipedeGame::clock % 8 == 0)
		//gameHandle->spawnObject<Bullet>(currentPosition.x, currentPosition.y);
	
	//Move the player based off the velocity.
	//Only allow the play to move every eight frames to a stuttered look.
	if (CentipedeGame::clock % 8 == 0
		&& gameHandle->isInBounds(currentPosition.x + velocity.x, currentPosition.y))
			currentPosition.x += velocity.x;
	if (CentipedeGame::clock % 8 == 0 
		&& gameHandle->isInBounds(currentPosition.x, currentPosition.y + velocity.y))
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