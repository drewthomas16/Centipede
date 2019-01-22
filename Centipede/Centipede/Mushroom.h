#pragma once
#include <SFML\Audio.hpp>
#include "GameObject.h"


class Mushroom : public GameObject
{
public:
	Mushroom(int, int);
	~Mushroom();
	void update(CentipedeGame *gameHandle);
	bool resetHeath();
	void collideWith(GameObject*);
	bool getPoisoned();
	void push();
	bool isPushed();
	char* getType() { return "Mushroom"; }
private:
	bool poisoned, pushed;
};