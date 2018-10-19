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
	getSprite()->setOrigin(1, 1);
	health = 3;
	shotBullet = false;
	xStart = x;
	yStart = y;
	objectsPtr = nullptr;
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
	
	//Get player hitbox adjusted for velocity and check collision with new box.
	if (objectsPtr != nullptr)
	{
		//Get a bound box for the future position of the ship.
		//We ignore magnitude of actual velocity and assume its 3 to avoid collision bugs.

		//Initial values based off of ship initial position.
		double boxLeft = object.getGlobalBounds().left;
		double boxTop = object.getGlobalBounds().top;
		double boxWidth = object.getGlobalBounds().width;
		double boxHeight = object.getGlobalBounds().height;

		//Values to adjust So that the future bounds have room.
		//They also need to be rounded because sprite has bounds of ints.
		double adjustVeloX = velocity.x;
		double adjustVeloY = velocity.y;
		
		//Make all magnitudes 2 and adjust for direction, account for if the velocity
		//is zero.
		if (adjustVeloX < 0)
			adjustVeloX = -2;
		else if (adjustVeloX > 0)
			adjustVeloX = 2;
		else
			adjustVeloX = 0;

		if (adjustVeloY < 0)
			adjustVeloY = -2;
		else if (adjustVeloY > 0)
			adjustVeloY = 2;
		else
			adjustVeloY = 0;

		//Make future bounds depending upon velocity.
		sf::FloatRect futurePosRect(boxLeft + adjustVeloX, boxTop + adjustVeloY,
			boxWidth + adjustVeloX , boxHeight + adjustVeloY);
		
		
		//std::cout << '(' << adjustVeloX << ',' << adjustVeloY << std::endl;

		//Cycle through all mushrooms, the 3rd index of the objects vector.
		for (int i = 0; i < (objectsPtr + 3)->size(); ++i)
		{
			//If the future position collides with a mushroom.
			if ((objectsPtr + 3)->at(i)->getSprite()->getGlobalBounds().intersects(futurePosRect))
			{
				//Stop all motion.
				velocity.x = 0;
				velocity.y = 0;
			}
		}



	}

	//Move the player based off the velocity.
	//Only allow the play to move every eight frames to a stuttered look.
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

	currentPosition.x = xStart;
	currentPosition.y = yStart;

	//play a death anim here
}}

void Player::setObjectsPtr(std::vector<std::shared_ptr<GameObject>>* entitylist)
{
	objectsPtr = entitylist;
}
