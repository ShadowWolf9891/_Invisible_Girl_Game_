#pragma once
//
//#include "Invisible_Girl_Game.h"
//
//class NPC : public sf::Sprite
//{
//private:
//	//Variables
//	int moveDirectionX, moveDirectionY; //1 is foreward, -1 is backwards, 0 is stopped
//	float maxSpeed;
//	int curTexture, frameDelay, frameCounter;
//	std::vector<sf::Texture> tList;
//	std::vector<sf::IntRect> spriteRects;
//
//	//Functions
//	virtual void init();
//	virtual void moveNPC(float elapsedTime, float friction);
//	virtual void animateNPC(float elapsedTime);
//
//public:
//	//Constructor / Deconstructor
//	NPC();
//	virtual ~NPC();
//
//	//Variables
//	float xSpeed, ySpeed, xAcceleration, yAcceleration, xPos, yPos;
//	bool isVisible = false;
//	int CHARACTER_TYPE = GLC::UNASSIGNED_CHARACTER;
//
//	//Functions
//	virtual void spawnNPC(sf::Vector2f initLocation);
//	virtual void interactWith(Sprite targetSprite);
//	virtual void updateNPC(float elapsedTime, float friction = 1);
//	virtual void addTexture(sf::Texture t);
//	
//};
//
