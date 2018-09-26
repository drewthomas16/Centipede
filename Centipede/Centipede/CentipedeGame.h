#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <vector>
#include "GameObject.h"
#include "Player.h"
#include "Flea.h"
#include <fstream>
#include <memory>
#include <typeinfo>
#include <string>

class CentipedeManager;

class CentipedeGame
{
public:
	enum objectTypes
	{
		player = 0,
		bullet,
		centipedeSegment,
		mushroom,
		flea,
		scorpion,
		spider
	};

	const std::string pobjectTypes[7] = { "class Player", "class Bullet",
		"class CentipedeSegment", "class Mushroom", "class Flea",
		"class Scorpion", "class Spider" };

	CentipedeGame(sf::RenderWindow *, const sf::Vector2u);
	~CentipedeGame();

	bool update();
	void draw();
	static bool isMushroomCell(double x, double y);
	void reset();
	void placeObject(unsigned int, unsigned int, std::shared_ptr<GameObject>);
	sf::Vector2i getRelMousePos();
	static bool isInBounds(double x, double y) { return x < 30 && x >= 0 && y < 30 && y >= 0; }

	template <typename type> std::shared_ptr<type> spawnObject(double x, double y) 
	{
		std::shared_ptr<type> thing(nullptr);
		std::string name = typeid(type).name();

		if (isInBounds(x, y)) 
		{
			thing = std::make_shared<type>(x, y);
			
			for (int i = 0; i < numObjects; i++)
				if (pobjectTypes[i] == name)
					objects[i].push_back(thing);
		}
		return thing;
	};

	static unsigned int clock;

	unsigned int getCountOf(char*, unsigned int, unsigned int, unsigned int, unsigned int);

private:
	static const int numObjects = 7;
	void resolveCollisions();
	void generateGrid();

	void manageCentipedePopulation();


	void kill(std::shared_ptr<GameObject>&);
	sf::VertexArray linePoints;

	static bool frame;

	//refer to the enum GameObjectType to see where each object type is located.
	static std::vector<std::shared_ptr<GameObject>> objects[numObjects];


	sf::RenderWindow * window = nullptr;
	const sf::Vector2u originalWindowDimensions;
	static unsigned int score;
	static int playerLives;

	sf::Font arcadeFont;
	sf::Text scoreDisplay;

	sf::Texture lifeTexture;
	sf::Sprite lives[6];
	sf::RenderTexture playerArea;
	sf::RenderTexture scoreArea;
	sf::Sprite playerAreaSprite;
	sf::Sprite scoreAreaSprite;
	sf::Sprite startingScreen;

	void drawLives();

	bool activeCentipede = false;

	std::ifstream score_in_file;
	std::ofstream score_out_file;

	CentipedeManager *centMan;

	template <class type> std::shared_ptr<type> findFirstInstanceOf() {
		std::shared_ptr<type> instance = nullptr;
		for(int i = 0; i < numObjects; i++)
			for (int j = 0; j < objects[i].size() && instance == nullptr; j++)
				if (std::dynamic_pointer_cast<type> (objects[i].at(j)))
					instance = std::dynamic_pointer_cast<type> (objects[i].at(j));

		return instance;
	}
};