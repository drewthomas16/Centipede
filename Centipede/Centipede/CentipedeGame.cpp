#pragma once
#include "stdafx.h"
#include <iostream>
#include "CentipedeGame.h"
#include "Mushroom.h"
#include "Player.h"
#include "Scorpion.h"
#include "CentipedeSegment.h"
#include "Spider.h"
#include "CentipedeManager.h"
#include "Player.h"
#include "Mushroom.h"

bool CentipedeGame::frame = false;
//Vector of all of the entities shown on the map.
std::vector<std::shared_ptr<GameObject>> CentipedeGame::objects[numObjects];
unsigned int CentipedeGame::clock = 0, CentipedeGame::score = 0;
int CentipedeGame::playerLives = -1;
static int lastPlayerLives;

int numObjects;


//Create centipede game loop.
CentipedeGame::CentipedeGame(sf::RenderWindow * renderWindow,
	const sf::Vector2u oWD) : originalWindowDimensions(oWD)
{
	GameObject::oWD = oWD;
	window = renderWindow;

	//create player
	std::shared_ptr<Player> player = spawnObject<Player>(15.0, 29.0);

	//randomly place mushrooms on map on startup
	for (int y = 0; y < 29; ++y)
		for (int x = 0; x < 30; ++x)
			if (rand() % (rand() % 35 + 1) == 1)
				spawnObject<Mushroom>(x, y);

	//create sprites for drawing 
	scoreArea.create(renderWindow->getSize().x, renderWindow->getSize().x * .05);
	playerArea.create(renderWindow->getSize().x, renderWindow->getSize().y);
	playerAreaSprite.setTexture(playerArea.getTexture());
	scoreAreaSprite.setTexture(scoreArea.getTexture());
	playerAreaSprite.move(0, renderWindow->getSize().x * .05);

	//load score fonts and stuff
	arcadeFont.loadFromFile("../ARCADECLASSIC.TTF");
	scoreDisplay.setFont(arcadeFont);
	scoreDisplay.setCharacterSize(18);

	//life display
	lifeTexture.loadFromFile("../Sprites/player.png");
	for (int i = 0; i < 6; i++)
	{
		lives[i].setTexture(lifeTexture);
		lives[i].setPosition(10 + 20 * i, 0);
	}

	centMan = new CentipedeManager();
	centMan->bindToGame(this);
	centMan->beginSpawn(CentipedeGame::clock, 8, 8);
}


//Clear the game so there is no memory problems.
CentipedeGame::~CentipedeGame()
{
	for(int i = 0; i < numObjects; i++)
		objects[i].clear();

	delete centMan;
}


static bool liveFlea = false;
//Make sure all the rules are being followed and update positons.
bool CentipedeGame::update()
{
	for(int i = 0; i < numObjects; i++)
		for (int j = 0; j < objects[i].size(); j++)
			objects[i].at(j)->update(this);

	frame = !frame;

	resolveCollisions();

	//remove items with 0 health
#pragma region mapCleanup
	for(int i = 0; i < numObjects; i++)
		for (int j = 0; j < objects[i].size(); j++)
			if (objects[i].at(j)->getHealth() == 0)
			{
				kill(objects[i].at(j));
				objects[i].erase(objects[i].begin() + j);
			}
#pragma endregion

	//update player health display
	lastPlayerLives = playerLives;
	playerLives = objects[player].at(0)->getHealth();

	//check if flea needs to be spawned
	#pragma region fleaCheck
	int mushroomCount = 0;
	for (int y = 28; y > 17; y--)//check mushrooms in player position
		for (int x = 0; x < 29; x++)
			if (isMushroomCell(x, y))
				++mushroomCount;

	if (mushroomCount < 5 && !liveFlea)
	{
		int xpos = rand() % 29;
		spawnObject<Flea>(xpos, 0);
		liveFlea = true;
	}
	#pragma endregion*/

	if (!findFirstInstanceOf<Scorpion>() && rand() % 10000 < 5)
		spawnObject<Scorpion>(rand() % 30 < 15 ? 0 : 29, rand() % 17);

	//check if live spider
	if (!findFirstInstanceOf<Spider>() && (rand() % 2000) < 5)//no spider check if able to respawn
	{
		std::shared_ptr<Spider> spider = spawnObject<Spider>(rand() 
			% 30 < 15 ? 0 : 29, rand() % 5 + 18);
		spider->setTarget(findFirstInstanceOf<Player>());
	}

	//this should be happening when player dies
#pragma region rebuildMushroom
	if (lastPlayerLives > playerLives)//player dies
	{
		for (int i = 0; i < objects[mushroom].size(); i++)
			while (objects[mushroom].at(i)->getHealth() < 4)
			{
				draw();
				std::dynamic_pointer_cast<Mushroom> (objects[mushroom].at(i))->resetHeath();
				std::cout << "help" << objects[mushroom].at(i)->getHealth();
			}

		//killCentipedes();
	}
#pragma endregion

	manageCentipedePopulation();
	centMan->update();

	draw();

	++clock;

	return playerLives > 0;//return true while player alive
}


static bool grid = false;
//Draw the board.
void CentipedeGame::draw()
{
	//this is temp
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		grid = !grid;

	//window->clear();
	scoreArea.clear();
	playerArea.clear();

	//update score and draw to render texture
	scoreDisplay.setFillColor(sf::Color::Red);
	scoreDisplay.setString("Score " + std::to_string(score));
	scoreDisplay.setOrigin(scoreDisplay.getLocalBounds().width / 2, scoreDisplay
		.getLocalBounds().height / 2);
	scoreDisplay.setPosition(scoreAreaSprite.getTexture()->getSize().x / 2, 0);

	scoreArea.draw(scoreDisplay);
	//draw lives
	drawLives();

	if (grid)
		playerArea.draw(linePoints);

	playerArea.display();
	scoreArea.display();

	GameObject::interval = static_cast<sf::Vector2i>(playerArea.getSize()) / 30;

	for(int i = 0; i < numObjects; i++)
		for (int j = 0; j < objects[i].size(); j++)
			objects[i].at(j)->render(playerArea);

	window->draw(playerAreaSprite);
	window->draw(scoreAreaSprite);
	window->display();
}


//Check to see if there is a mushroom in a certain x and y location.
bool CentipedeGame::isMushroomCell(double x, double y)
{
	if (x < 30 && y < 30)
		for (int i = 0; i < objects[mushroom].size(); i++)
			if (objects[mushroom].at(i)->currentPosition.x == x
				&& objects[mushroom].at(i)->currentPosition.y == y)
				return true;
	return false;
}


//start a level
void CentipedeGame::reset()
{

}


//if any index in map has more than 1 object in vector then deal with it.
void CentipedeGame::resolveCollisions()
{
	//Player
	for (int i = 0; i < objects[player].size(); i++)
	{
		//to Mushroom
		for (int j = 0; j < objects[mushroom].size(); j++)
			if (objects[player].at(i)->getSprite()->getGlobalBounds().intersects(objects[mushroom].at(j)->getSprite()->getGlobalBounds()))
				objects[player].at(i)->collideWith(objects[mushroom].at(j).get());
		//to CentipedeSegment
		for (int j = 0; j < objects[centipedeSegment].size(); j++)
			if (objects[player].at(i)->getSprite()->getGlobalBounds().intersects(objects[centipedeSegment].at(j)->getSprite()->getGlobalBounds()))
				objects[player].at(i)->collideWith(objects[centipedeSegment].at(j).get());//to Spider
		//to Spider
		for (int j = 0; j < objects[spider].size(); j++)
			if (objects[player].at(i)->getSprite()->getGlobalBounds().intersects(objects[spider].at(j)->getSprite()->getGlobalBounds()))
				objects[player].at(i)->collideWith(objects[spider].at(j).get());
		//to Scorpion
		for (int j = 0; j < objects[scorpion].size(); j++)
			if (objects[player].at(i)->getSprite()->getGlobalBounds().intersects(objects[scorpion].at(j)->getSprite()->getGlobalBounds()))
				objects[player].at(i)->collideWith(objects[scorpion].at(j).get());
		//to Flea
		for (int j = 0; j < objects[flea].size(); j++)
			if (objects[player].at(i)->getSprite()->getGlobalBounds().intersects(objects[flea].at(j)->getSprite()->getGlobalBounds()))
				objects[player].at(i)->collideWith(objects[flea].at(j).get());

	}

	//Bullet
	for (int i = 0; i < objects[bullet].size(); i++)
	{
		//to Mushroom
		for (int j = 0; j < objects[mushroom].size(); j++)
			if (objects[bullet].at(i)->getSprite()->getGlobalBounds().intersects(objects[mushroom].at(j)->getSprite()->getGlobalBounds()))
   				objects[bullet].at(i)->collideWith(objects[mushroom].at(j).get());
		//to CentipedeSegment
		for (int j = 0; j < objects[centipedeSegment].size(); j++)
			if (objects[bullet].at(i)->getSprite()->getGlobalBounds().intersects(objects[centipedeSegment].at(j)->getSprite()->getGlobalBounds()))
				objects[bullet].at(i)->collideWith(objects[centipedeSegment].at(j).get());
		//to Spider
		for (int j = 0; j < objects[spider].size(); j++)
			if (objects[bullet].at(i)->getSprite()->getGlobalBounds().intersects(objects[spider].at(j)->getSprite()->getGlobalBounds()))
				objects[bullet].at(i)->collideWith(objects[spider].at(j).get());
		//to Scorpion
		for (int j = 0; j < objects[scorpion].size(); j++)
			if (objects[bullet].at(i)->getSprite()->getGlobalBounds().intersects(objects[scorpion].at(j)->getSprite()->getGlobalBounds()))
				objects[bullet].at(i)->collideWith(objects[scorpion].at(j).get());
		//to Flea
		for (int j = 0; j < objects[flea].size(); j++)
			if (objects[bullet].at(i)->getSprite()->getGlobalBounds().intersects(objects[flea].at(j)->getSprite()->getGlobalBounds()))
				objects[bullet].at(i)->collideWith(objects[flea].at(j).get());
	}
}


//Put an object on the new frame.
void CentipedeGame::placeObject(unsigned int x, unsigned int y,
	std::shared_ptr<GameObject> object)
{
	//keep object in bounds of array
	if (x < 30 && y < 30)
	{
		//objects.push_back(object);
	}
	else
		kill(object);
}


//Kill an object that needs to and output useful information.
void CentipedeGame::kill(std::shared_ptr<GameObject>& thing) 
{
	bool readyToDie;
	std::cout << "i exist " << thing.use_count() << " times\n";

  	score += thing->die(readyToDie, this);

	if (readyToDie)
		//thing.reset();

	std::cout << "score is now " << score << std::endl;
}

/*
//Make a grid that the rest of the game can use.
void CentipedeGame::generateGrid() 
{
	int scalar = originalWindowDimensions.x / 30;
	sf::Color col(20, 20, 20);

	for (int i = 0, index = 0; i < 30; ++i, index += 4) {
		linePoints[index + 0] = sf::Vector2f(0, i*scalar);
		linePoints[index + 1] = sf::Vector2f(originalWindowDimensions.x, i*scalar);
		linePoints[index + 2] = sf::Vector2f(i*scalar, 0);
		linePoints[index + 3] = sf::Vector2f(i*scalar, originalWindowDimensions.y);

		for (int offset = 0; offset < 4; ++offset)
			linePoints[index + offset].color = col;
	}
}
*/

//Count how many things you have. You must specify which thing you want to count.
unsigned int CentipedeGame::getCountOf(char* type, unsigned int startX = 0, 
	unsigned int startY = 0, unsigned int endX = 30, unsigned int endY = 30) 
{
	unsigned int count = 0;
	/*
	for (int y = startY; y < endY; ++y)//check mushrooms in player position
		for (int x = startX; x < endX; ++x)
			for (int i = 0; i < CentipedeGame::map[y][x][CentipedeGame::frame]
				.size(); i++)
				if (!std::strcmp(CentipedeGame::map[y][x][CentipedeGame::frame]
					.at(i)->getType(), type))

					++count;*/
	return count;
}


// If the getCounOf("CentipedeSegment", 0, 0, 30, 30) returns a number less than
//0 then you know that there is no longer a centipede on the board.
void CentipedeGame::manageCentipedePopulation() 
{
	if (activeCentipede) {
		//check if centipede has died
		activeCentipede = getCountOf("CentipedeSegment", 0, 0, 30, 30) > 0;
	}
	else {

		//centMan.beginSpawn()
	}
}


//Ouput how many lives a player has.
void CentipedeGame::drawLives()
{
	for (int i = 0; i < playerLives; i++)
		scoreArea.draw(lives[i]);
}


//Get the position of the mouse.
sf::Vector2i CentipedeGame::getRelMousePos() 
{
	sf::Vector2f mousePos(sf::Mouse::getPosition(*window));
	float scalar = static_cast<float>(GameObject::oWD.x) / window->getSize().x;
	mousePos *= scalar;
	return sf::Vector2i(mousePos);
}