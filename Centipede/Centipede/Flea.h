#pragma once
#include "GameObject.h"


class Flea : public GameObject
{
public:
	Flea();
	Flea(int, int);
	~Flea();
	void update(CentipedeGame*);
	void collideWith(GameObject * other);
	char* getType() { return "Flea"; }
private:
	//Tick variable for flea.
	unsigned int delay = 2;
};