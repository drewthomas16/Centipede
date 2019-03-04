#pragma once
#include "GameObject.h"
#include <iostream>


class Bullet : public GameObject
{
public:
	Bullet(double, double);
	~Bullet();
	void update(CentipedeGame*);
	void collideWith(GameObject*);
	unsigned int die(bool&, CentipedeGame*);
	char* getType() { return "Bullet"; }
	static bool liveBullet;
private:
	int delay = 1;
	double xStart;
	double yStart;
};