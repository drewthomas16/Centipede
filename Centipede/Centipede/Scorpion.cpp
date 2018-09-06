#include "stdafx.h"
#include "Scorpion.h"
#include "GameObject.h"
#include "Bullet.h"

//Scorpion is a GameObject
Scorpion::Scorpion(int x, int y) : GameObject(x, y)
{
	pointValue = 1000;
	health = 1;
	scuttle = 0;
	//Is a larger Object.
	object.setScale(sf::Vector2f(1.5, 1.5));
	velocity = sf::Vector2f(
		/* if x < 15 then the x value for velocity is 1 otherwise it will be -1*/
		x < 15 ? 1 : -1 
		,0);
}


static unsigned int frame = 0;
//Updates a Scorpion entidy's status every tick? (makes the most sense)
void Scorpion::update(CentipedeGame *gameHandle)
{
	//Sets the sprite and position.
	setPixels();
	//Handles each frame of animation for the scorpion.
	if (frame++ == frameMax)
	{
		//Guessing this was for testing?
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))//you see nothing
		{
			currentPosition.y--;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))//you see nothing
		{
			currentPosition.y++;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))//you see nothing
		{
			currentPosition.x--;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))//you see nothing
		{
			currentPosition.x++;
		}
		else
		{
			currentPosition.x += velocity.x;
		}

		if (scuttle++ >= 3)//dancing
			scuttle = 0;

		if (scuttle == 0)                                     //dancing
			setTexture("../Sprites/Scorpion/scorpionLeft1.png");
		else if (scuttle == 1)
			setTexture("../Sprites/Scorpion/scorpionLeft2.png");
		else if (scuttle == 2)
			setTexture("../Sprites/Scorpion/scorpionRight1.png");
		else
			setTexture("../Sprites/Scorpion/scorpionRight2.png");

		if (currentPosition.x == 0 || currentPosition.x == 29)
		{
			pointValue = 0;
			health = 0;//kill if offscreen
		}

		frame = 0;
	}
}

//Is called in CentipedeGame if it detects multiple objects in the same pos.
void Scorpion::collideWith(GameObject* other)
{
	//Kill scorpion if what it collides with is not a nullptr.
	if (dynamic_cast<Bullet*>(other) != nullptr)
		health = 0;
}