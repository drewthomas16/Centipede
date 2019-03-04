#include "stdafx.h"
#include "Flea.h"
#include "CentipedeGame.h"
#include "Mushroom.h"


Flea::Flea()
{
}


Flea::Flea(int x, int y) : GameObject(x, y)
{
	setTexture("../Sprites/flea.png");
	object.setScale(sf::Vector2f(.5, .5));
	//object.setOrigin(-6, -10);
	object.setPosition(sf::Vector2f(0, 0));
	velocity = sf::Vector2f(0, 1);
	pointValue = 200;
	health = 2;
}


Flea::~Flea()
{
}


static unsigned int frame = 0;
void Flea::update(CentipedeGame *gameHandle)
{
	//Converts grid-based position to sfml screen coordinates.
	setPixels();

	if (frame++ == delay)//if time to update
	{
		//Gets faster if hit by a bullet.
		if (health == 1)
			velocity.y = 2;

		//apply velocity
		currentPosition.y += velocity.y;

		//reset frame counter
		frame = 0;

		//Flea places mushrooms at random locations along its path.
		if (rand() % 100 < 30 &&
			!CentipedeGame::isMushroomCell(currentPosition.x, currentPosition.y))
			gameHandle->spawnObject<Mushroom>(currentPosition.x, currentPosition.y);

		//check if bottom of screen
		if (currentPosition.y >= 29)
		{
			pointValue = 0;
			health = 0;
		}
	}

}

void Flea::collideWith(GameObject * other)
{
	if (dynamic_cast<Bullet *>(other) != nullptr)
		health--;
}