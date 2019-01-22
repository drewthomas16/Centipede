#pragma once
#include "GameObject.h"


class Scorpion : public GameObject
{
public:
	//Constructor takes a position.
	Scorpion(int, int);
	//Handles scorpion animation and kills if offscreen.
	void update(CentipedeGame *gameHandle);
	//Called by CentipedeGame if collisions occur.
	void collideWith(GameObject*);
	char* getType() { return "Scorpion"; }
private:
	int scuttle; // for walking animation

	//40 frames of animation.
	unsigned int frameMax = 5;
};