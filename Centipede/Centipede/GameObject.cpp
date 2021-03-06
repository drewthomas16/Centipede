#include "stdafx.h"
#include "GameObject.h"
#include "CentipedeGame.h"
#include <math.h>
#include <iostream>

sf::Vector2u GameObject::oWD = sf::Vector2u(0, 0);
sf::Vector2i GameObject::interval = sf::Vector2i(1, 1);




//Constructor to set x and y positions and play sound.
GameObject::GameObject(int x, int y)
{
	currentPosition = sf::Vector2f(x, y);
	//object.setOrigin(currentPosition);
	object.setPosition(currentPosition);
	realColor = object.getColor();
	soundPlayer.setBuffer(soundClip);
}


//Method to to draw the object in a playerArea.
void GameObject::render(sf::RenderTexture & playerArea)
{
	setPixels();
	playerArea.draw(object);
}


//Method to check if GameObjects collide.
void GameObject::collideWith(GameObject *)
{
	return;
}




//Method to give more health to the object.
void GameObject::addHealth(int health)
{
	return;
}


//Method to move an object to another block.
sf::Vector2f GameObject::getNearestCellPos(sf::Vector2f position)
{
	position.x -= fmod(position.x, interval.x);
	position.y -= fmod(position.y, interval.y);
	 
	return position;
}


//Method to set the position of the object.
void GameObject::setPixels()
{
	object.setPosition(currentPosition.x 
		* interval.x, currentPosition.y 
		* interval.y);
}


sf::Sprite* GameObject::getSprite()
{
	sf::Vector2f realPos(currentPosition.x
		* interval.x, currentPosition.y
		* interval.y);
	object.setPosition(realPos);
	
	return &object;
}

//Method to kill the object.
unsigned int GameObject::die(bool &readyToDie, CentipedeGame *gameHandle) 
{
	readyToDie = true;
	
	return getPointValue();
}

sf::Color GameObject::getColor()
{
	return realColor;
}

void GameObject::setColor(sf::Color claim)
{
	realColor = claim;
}


//Method to return the objects type.
char* GameObject::getType() 
{
	return "GameObject";
}



//Getters
int GameObject::getPointValue()
{
	return pointValue;
}
unsigned int GameObject::getHealth() const
{
	return health;
}
sf::Vector2f GameObject::getVelocity() const
{
	return velocity;
}
sf::Vector2f GameObject::getPosition() const
{
	return currentPosition;
}


//Setters
void GameObject::setTexture(const char * filePath)
{
	if (!textureTemp.loadFromFile(filePath))
		printf("Errors\n");
	object.setTexture(textureTemp);
}
void GameObject::setPointValue(unsigned int newPointValue)
{
	pointValue = newPointValue;
}
void GameObject::setVelocity(sf::Vector2f newVel)
{
	velocity = newVel;
}


//Default constructor is not being used.
GameObject::GameObject()
{
}

//Default deconstructor is not being used.
GameObject::~GameObject()
{
}