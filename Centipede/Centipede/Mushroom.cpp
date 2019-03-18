#include "stdafx.h"
#include "Mushroom.h"
#include "Spider.h"
#include "Scorpion.h"
#include "Bullet.h"


Mushroom::Mushroom(int x, int y) : GameObject(x, y)
{
	pointValue = 5;
	poisoned = false;
	pushed = false;
	health = 4;

	object.setOrigin(5, 0);
	
	velocity.y = 0.5;
}


//Method to update the mushroom to make sure it is
//representing how many times it has been shot.
void Mushroom::update(CentipedeGame *gameHandle)
{	
	switch (health)//changes texture based on health
	{
	case 1:
		setTexture("../Sprites/Mushroom/mushroom1.png");
		break;
	case 2:
		setTexture("../Sprites/Mushroom/mushroom2.png");
		break;
	case 3:
		setTexture("../Sprites/Mushroom/mushroom3.png");
		break;
	case 4:
		setTexture("../Sprites/Mushroom/mushroom4.png");
		break;
	}

	if (poisoned)
	{
		//check if pushed  by centipede
		//if true go down unless at player area.
		setColor(sf::Color::Magenta);
		if (pushed && currentPosition.y < 29)
		{
			currentPosition.y += velocity.y;
		}
			
	}
}





//Make sure that the Mushroom is ready for the next level.
bool Mushroom::resetHeath()
{
	static sf::Sound mushroomRebuildSound;
	static sf::SoundBuffer rebuildSound;

	rebuildSound.loadFromFile("../Audio/mushroomReset.ogg");
	mushroomRebuildSound.setBuffer(rebuildSound);

	//Lower volume because sound is grating.
	mushroomRebuildSound.setVolume(40);
	if (health < 4)
	{

		switch (health++)//changes texture based on health
		{
		case 1:
			setTexture("../Sprites/Mushroom/Poisoned1.png");
			break;
		case 2:
			setTexture("../Sprites/Mushroom/Poisoned2.png");
			break;
		case 3:
			setTexture("../Sprites/Mushroom/Poisoned3.png");
			break;
		case 4:
			setTexture("../Sprites/Mushroom/Poisoned4.png");
			break;
		}

		mushroomRebuildSound.play();

		poisoned = false;
		return true;
	}
	else
		setTexture("../Sprites/Mushroom/Poisoned4.png");

	return false;
}


//Change what happens to the mushroom based off of what collided with the mushroom.
void Mushroom::collideWith(GameObject* other)
{
	if (dynamic_cast<Spider*>(other) != nullptr)
	{
		pointValue = 0;
		health = 0;
	}
	if (dynamic_cast<Scorpion*>(other) != nullptr)
		poisoned = true;
 	if (dynamic_cast<Bullet*>(other) != nullptr)
		health--;

}


//for centipede 
bool Mushroom::getPoisoned()
{
	return poisoned;
}

void Mushroom::push()
{
	if(poisoned)
		pushed = true;
}

bool Mushroom::isPushed()
{
	return pushed;
}


Mushroom::~Mushroom()
{
}