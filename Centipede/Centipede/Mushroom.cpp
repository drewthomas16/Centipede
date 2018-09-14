#include "stdafx.h"
#include "Mushroom.h"
#include "Spider.h"
#include "Scorpion.h"
#include "Bullet.h"


Mushroom::Mushroom(int x, int y) : GameObject(x, y)
{
	pointValue = 5;
	poisoned = false;
	health = 4;
	object.setOrigin(-2, -2);
	object.setScale(1.25, 1.25);
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
		object.setColor(sf::Color::Magenta);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		health = 0;
}





//Make sure that the Mushroom is ready for the next level.
bool Mushroom::resetHeath()
{
	static sf::Sound mushroomRebuildSound;
	static sf::SoundBuffer rebuildSound;

	rebuildSound.loadFromFile("../Audio/mushroomReset.ogg");
	mushroomRebuildSound.setBuffer(rebuildSound);

	if (health < 4)
	{
		switch (++health)//changes texture based on health
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

		mushroomRebuildSound.play();

		while (mushroomRebuildSound.getStatus() != sf::Sound::Stopped)
		{
			//printf("Health - %i\n", health);
		}
		poisoned = false;
		return true;
	}
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
		--health;

	std::cout << health << std::endl;
}


//for centipede 
bool Mushroom::getPoisoned()
{
	return poisoned;
}


Mushroom::~Mushroom()
{
}