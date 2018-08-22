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
	char* getType() { return "Mushroom"; }
private:
	bool poisoned;
};