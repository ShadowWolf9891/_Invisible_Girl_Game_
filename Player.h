//#pragma once
//
//#include "Invisible_Girl_Game.h"
//
//class Player : public sf::Sprite
//{
//private:
//	//Variables
//	bool moveUP, moveLEFT, moveDOWN, moveRIGHT;
//	bool stoppedUP, stoppedLEFT,stoppedDOWN, stoppedRIGHT;
//	int moveDirectionX = 0, moveDirectionY = 0; //1 is foreward, -1 is backwards, 0 is stopped
//	int currentLayer = 0; //Current layer the player is on, relative to the tiles and other objects
//	float maxSpeed;
//	int curTexture, frameDelay, frameCounter;
//	std::vector<sf::Texture> tList;
//	std::vector<sf::IntRect> spriteRects;
//	//Functions
//	void init();
//	void movePlayer(float elapsedTime, float friction);
//	void animatePlayer(float elapsedTime);
//	void unStop();
//	sf::IntRect mirrorRect(sf::IntRect rectToMirror);
//public:
//	//Constructors / Deconstructors
//	Player();
//	virtual ~Player();
//
//	//Variables
//	float xSpeed, ySpeed, xAcceleration, yAcceleration, xPos, yPos;
//	bool isVisible = false, canMoveX = true, canMoveY = true, isColliding = false;
//	int CHARACTER_TYPE = GLC::PLAYER;
//	
//	sf::Vector2f nextPosition;
//	sf::RectangleShape playerBoundingBox;
//	sf::Vector2f boxSize;
//	
//	//Functions
//	void readInput(Input input);
//	void updatePlayer(float elapsedTime, float friction = 1);
//	void addTexture(sf::Texture t);
//	void spawnPlayer(sf::Vector2f initLocation);
//
//	int getLayer(); //Gets the layer the player is on
//	void setLayer(int layer); //Sets the layer the player is on
//	
//};
//
