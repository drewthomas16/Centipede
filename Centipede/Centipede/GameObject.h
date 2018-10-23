#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <memory>

class CentipedeGame; //Forward declaration like a prototype

class GameObject
{
public:
	GameObject();
	GameObject(int, int);
	~GameObject();
	int getPointValue();
	unsigned int getHealth() const;
	sf::Vector2f getVelocity() const;
	sf::Vector2f getPosition() const;
	void setTexture(const char *);
	virtual void update(CentipedeGame*) = 0;
	void render(sf::RenderTexture &);
	virtual void collideWith(GameObject *);
	//virtual void collideWith(std::vector<GameObject *> *);
	sf::Vector2f getNearestCellPos(sf::Vector2f);

	static sf::Vector2u oWD;
	static sf::Vector2i interval;

	virtual sf::Sprite* getSprite();
	virtual unsigned int die(bool&, CentipedeGame*);

	char* getType();
	sf::Vector2f currentPosition;
protected:
	void setPointValue(unsigned int);
	void setVelocity(sf::Vector2f);
	void addHealth(int);
	void setPixels();

	sf::Sprite object;
	unsigned int health = 1;
	unsigned int pointValue = 0;
	sf::Vector2f velocity;
	sf::Texture textureTemp;

	sf::Sound soundPlayer;
	sf::SoundBuffer soundClip;

};