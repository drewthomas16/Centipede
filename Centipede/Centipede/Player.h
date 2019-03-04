#pragma once
#include "GameObject.h"
#include "Bullet.h"

class Player : public GameObject
{
public:
	Player(int, int);
	void update(CentipedeGame *gameHandle);
	char* getType() { return "Player"; }
	void collideWith(GameObject *);
	void setObjectsPtr(std::vector<std::shared_ptr<GameObject>>*);
	void checkMushroomCollision();

protected:
	std::shared_ptr<Bullet> bullet;
	double movementSpeed = 0.25;
private:
	//Boolean that stores if a bullet was shot.
	//Used to make sure player can't hold down shoot button.
	bool shotBullet;

	//Stores the initial start position from Player.
	int xStart, yStart;

	std::vector<std::shared_ptr<GameObject>> * objectsPtr;
};