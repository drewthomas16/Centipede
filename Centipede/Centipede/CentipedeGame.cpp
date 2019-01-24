#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <SFML\Window\Keyboard.hpp>
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
	scoreDisplay.setFillColor(sf::Color::Red);

	enemyScore.setFont(arcadeFont);
	enemyScore.setCharacterSize(20);
	enemyScore.setFillColor(sf::Color::White);

	//life display
	lifeTexture.loadFromFile("../Sprites/player.png");
	for (int i = 0; i < 6; i++)
	{
		lives[i].setTexture(lifeTexture);
		lives[i].setPosition(10 + 20 * i, 0);
	}

	//Set highscore
	std::string line;
	std::ifstream highScoreFile("Scores.txt");

	if (highScoreFile.is_open())
	{
		getline(highScoreFile, line);
		highScoreFile.close();
	}
	std::stringstream i(line);
	i >> highScore;

	level = 0;

	centMan = new CentipedeManager();
	centMan->bindToGame(this);
}


//Clear the game so there is no memory problems.
CentipedeGame::~CentipedeGame()
{
	delete centMan;
	setHighScore();
}


static bool liveFlea = false;
//Make sure all the rules are being followed and update positons.
bool CentipedeGame::update()
{
	for (int i = 0; i < numObjects; i++)
		for (int j = 0; j < objects[i].size(); j++)
			objects[i].at(j)->update(this);

	frame = !frame;

	resolveCollisions();

	//remove items with 0 health
#pragma region mapCleanup
	for (int i = 0; i < numObjects; i++)
		for (int j = 0; j < objects[i].size(); j++)
			if (objects[i].at(j)->getHealth() == 0)
			{
				kill(objects[i].at(j));
 				objects[i].erase(objects[i].begin() + j);
				if (i == 0)
				{	
					setHighScore();
					return false;
				}
			}
#pragma endregion
	//update player health display
	if (objects[player].at(0) != NULL)
	{
		lastPlayerLives = playerLives;
		playerLives = objects[player].at(0)->getHealth();
	}

	//check if flea needs to be spawned
#pragma region fleaCheck
	int mushroomCount = 0;
	for (int y = 28; y > 17; y--)//check mushrooms in player position
		for (int x = 0; x < 29; x++)
			if (isMushroomCell(x, y))
				++mushroomCount;

	
	//DEBUG: if(sf::Keyboard::isKeyPressed(sf::Keyboard::E) && clock % 2 == 0)
	if (mushroomCount < 5 && !liveFlea)
	{
		int xpos = rand() % 29;
		spawnObject<Flea>(xpos, 0);
		liveFlea = true;
	}
#pragma endregion
	// DEBUG: */if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && clock % 2 == 0)
	if (!findFirstInstanceOf<Scorpion>() && rand() % 10000 < 5)
		spawnObject<Scorpion>(rand() % 30 < 15 ? 0 : 29, rand() % 17);

	//check if live spider
	//DEBUG if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && clock % 2 == 0)
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
				//draw();
				std::dynamic_pointer_cast<Mushroom> (objects[mushroom].at(i))->resetHeath();
			}

		//killCentipedes();
	}
#pragma endregion
	centMan->update();
	if (objects[centipedeSegment].size() == 0 && centMan->getEnd() <= -1)
	{
		centMan->clear();

		centMan->beginSpawn(CentipedeGame::clock, 1, 9 - level);
   		for (int i = 0; i < level; i++)
		{
			centMan->beginSpawn(CentipedeGame::clock, 2, 1);
		}
		level++;
	}
		

	draw();

	++clock;

	//return true while player alive
	return playerLives > 0;
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
	scoreDisplay.setOrigin(scoreDisplay.getLocalBounds().width / 2, 
		scoreDisplay.getLocalBounds().height / 2);
	scoreDisplay.setString("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" + std::to_string(score) 
		+ "\t\t\t\t\t\t\t\t\t\t\t\t" + "HI SCORE   " + std::to_string(highScore));
	scoreDisplay.setPosition(scoreAreaSprite.getTexture()->getSize().x / 2, 0);

	scoreArea.draw(scoreDisplay);
	scoreArea.draw(highScoreDisplay);

	//TODO: delete scoreText
	GameObject::interval = static_cast<sf::Vector2i>(playerArea.getSize()) / 30;
	playerArea.display();
	scoreArea.display();


	//Iterate through vector that holds all of data about entity deaths.
	for (unsigned int i = 0; i < toDisplay.size(); ++i)
	{
		//Determine how long its been since thing has died
		std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now()
			- toDisplay.at(i).timeOfDeath;
		//Don't draw if the death was more than one second ago.
		if (elapsed.count() <= 1)
		{
			//Give text variable all neccessary data and draw text.
			enemyScore.setPosition(toDisplay.at(i).deathPosition.x * GameObject::interval.x,
				toDisplay.at(i).deathPosition.y * GameObject::interval.y);
			enemyScore.setString(std::to_string(toDisplay.at(i).scoreValue));
			playerArea.draw(enemyScore);
			
		}
		//If death was too long ago kill data.
		else
		{
			toDisplay.erase(toDisplay.begin() + i);
			i--;
		}
		
	}
	//window->display();


	//draw lives
	drawLives();

	if (grid)
		playerArea.draw(linePoints);
	

	for(int i = 0; i < numObjects; i++)
		for (int j = 0; j < objects[i].size(); j++)
			objects[i].at(j)->render(playerArea);

	window->draw(playerAreaSprite);
	window->draw(scoreAreaSprite);
	window->display();
}


//Check to see if there is a mushroom in a certain x and y location.
//Takes non-real cell position, I dont think this can be used for player collision. 
//Problems with centipede collision?
bool CentipedeGame::isMushroomCell(double x, double y)
{
	
	if (x < 30 && y < 30)
		for (int i = 0; i < objects[mushroom].size(); i++)
			if (objects[mushroom].at(i)->currentPosition.x == x
				&& objects[mushroom].at(i)->currentPosition.y == y)
			{
				return true;
				
			}
				
	return false;
}


//if any index in map has more than 1 object in vector then deal with it.
void CentipedeGame::resolveCollisions()
{
	//Initializes objects for centipede
	for (int i = 0; i < objects[centipedeSegment].size(); ++i)
	{
		dynamic_cast<CentipedeSegment *>(objects[centipedeSegment].at(i).get())->setObjectsPtr(objects);
	}
	//Initialize objects for all players
	for (int i = 0; i < objects[player].size(); ++i)
	{
		dynamic_cast<Player *>(objects[player].at(i).get())->setObjectsPtr(objects);
	}



	//Player
	for (int i = 0; i < objects[player].size(); i++)
	{
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

	//Scorpion
	for (int i = 0; i < objects[scorpion].size(); ++i)
	{
		//Only Really Need to Check in relation to mushes.
		for (int j = 0; j < objects[mushroom].size(); ++j)
			if (objects[scorpion].at(i)->getSprite()->getGlobalBounds().intersects(objects[mushroom].at(j)->getSprite()->getGlobalBounds()))
				objects[mushroom].at(j)->collideWith(objects[scorpion].at(i).get());
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
 	int scoreAdd = thing->die(readyToDie, this);
 	score += scoreAdd;
 	if (scoreAdd > 0)
	{
 		DeathData deadThing;
		deadThing.scoreValue = scoreAdd;
		deadThing.deathPosition = thing->currentPosition;
		deadThing.timeOfDeath = std::chrono::high_resolution_clock::now();
		toDisplay.push_back(deadThing);
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


//Reset game
void CentipedeGame::reset()
{
	score = 0;

	delete centMan;

	for (int i = 0; i < 7; i++)
		for (int j = 0; j < objects[i].size(); j++)
			objects[i].clear();

	//create player
	std::shared_ptr<Player> player = spawnObject<Player>(15.0, 29.0);

	//randomly place mushrooms on map on startup
	for (int y = 0; y < 29; ++y)
		for (int x = 0; x < 30; ++x)
			if (rand() % (rand() % 35 + 1) == 1)
				spawnObject<Mushroom>(x, y);

	//load score fonts and stuff
	arcadeFont.loadFromFile("../ARCADECLASSIC.TTF");
	
	scoreDisplay.setFont(arcadeFont);
	scoreDisplay.setCharacterSize(18);
	scoreDisplay.setFillColor(sf::Color::Red);

	enemyScore.setFont(arcadeFont);
	enemyScore.setCharacterSize(15);
	enemyScore.setFillColor(sf::Color::Blue);

	//life display
	lifeTexture.loadFromFile("../Sprites/player.png");
	for (int i = 0; i < 6; i++)
	{
		lives[i].setTexture(lifeTexture);
		lives[i].setPosition(10 + 20 * i, 0);
	}

	level = 1;

	centMan = new CentipedeManager();
	centMan->bindToGame(this);
}


//Check to see high schore
void CentipedeGame::setHighScore()
{
	std::ofstream highScoreFile("Scores.txt", std::ofstream::out | std::ofstream::trunc);
	if (highScoreFile.is_open())
	{
		if (score > highScore)
		{
			highScoreFile << score;
			highScore = score;
		}
		else
			highScoreFile << highScore;

		highScoreFile.close();
	}
}