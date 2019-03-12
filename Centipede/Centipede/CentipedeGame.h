#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <vector>
#include <fstream>
#include <chrono>
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

	//A simple enum to seperate our entity list into class types.
	//We do this so that we only calculate collision for entities that can collide with eachother.
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

	//Strings so that object type can be output.
	const std::string pobjectTypes[7] = { "class Player", "class Bullet",
		"class CentipedeSegment", "class Mushroom", "class Flea",
		"class Scorpion", "class Spider" };

	CentipedeGame(sf::RenderWindow *, const sf::Vector2u);
	~CentipedeGame();

	//This is the general game loop for centipede.
	bool update();
	//Called within update, draws all required textures/sprites.
	void draw();

	//Used to check if there are mushrooms in a location. Maily used so that the game
	//can calculate how many mushrooms are currently on screen.
	static bool isMushroomCell(double x, double y);

	//Sets/resets the game by initializing key elements.
	void reset();

	//Gets the mouse position relative to the game window.
	sf::Vector2i getRelMousePos();
	//Checks if a coordinate is within bounds.
	static bool isInBounds(double x, double y) { return x < 30 && x >= 0 && y < 30 && y >= 0; }

	//Spawns an object and adds it to entity vector.
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

	//Clock value that controls tick rate.
	static unsigned int clock;

	//Sets high score.
	void setHighScore();

private:
	//The number of types of gameobjects that can exist.
	static const int numObjects = 7;
	//Check all bounds of relevent entities to see if collisions happen.
	//Then tell entities to have collision effects if they collide.
	void resolveCollisions();


	void kill(std::shared_ptr<GameObject>&);

	unsigned int level;

	//refer to the enum GameObjectType to see where each object type is located.
	static std::vector<std::shared_ptr<GameObject>> objects[numObjects];

	sf::Color screenModifiers[7];
	unsigned int screenColor;

	sf::RenderWindow * window = nullptr;
	const sf::Vector2u originalWindowDimensions;
	static unsigned int score;
	static int playerLives;
	unsigned int lastPlayerLives;
	unsigned int highScore;
	std::vector<double> scoreText;

	sf::Font arcadeFont;
	sf::Text scoreDisplay;
	sf::Text highScoreDisplay;
	sf::Text enemyScore;

	sf::Texture lifeTexture;
	sf::Sprite lives[6];
	sf::RenderTexture playerArea;
	sf::RenderTexture scoreArea;
	sf::Sprite playerAreaSprite;
	sf::Sprite scoreAreaSprite;
	sf::Sprite startingScreen;

	//Struct that holds all relevent data when an object dies.
	//Used to show score values on kill. Is only relevent when object's score > 10.
	struct DeathData
	{
		int scoreValue;
		sf::Vector2f deathPosition;
		std::chrono::high_resolution_clock::time_point timeOfDeath;
	};

	//Holds all death data and provides information to score popup feature.
	std::vector<DeathData> toDisplay;

	void drawLives();

	bool activeCentipede = false;
	bool firstLoop;

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