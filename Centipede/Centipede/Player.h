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
protected:
	std::shared_ptr<Bullet> bullet;
	double movementSpeed = 0.25;
private:
	bool shotBullet;
	int xStart, yStart;
	std::vector<std::shared_ptr<GameObject>> * objectsPtr;
};