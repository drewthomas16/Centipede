#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include <vector>

class Player : public GameObject
{
public:
	Player(int, int);
	void update(CentipedeGame *gameHandle);
	char* getType() { return "Player"; }
	void collideWith(GameObject *);
	void setThingPtr(std::vector<std::shared_ptr<GameObject>>*);
private:
	std::shared_ptr<Bullet> bullet;
	double movementSpeed = 0.25;
	bool shotBullet;
	std::vector<std::shared_ptr<GameObject>> *thingPtr = nullptr;
};

/*
   - Player will have a pointer to a vector with the locations of all gameobjects
   -Player's update will then calculate it's future position and not move if there is a collision.
*/