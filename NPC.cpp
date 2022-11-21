//#include "NPC.h"
//
//NPC::NPC()
//{
//	this->init();
//}
//
//NPC::~NPC()
//{
//	delete this;
//}
//void NPC::init()
//{
//
//
//}	
//
//void NPC::spawnNPC(sf::Vector2f initLocation)
//{
//
//
//}
//
//void NPC::interactWith(Sprite targetSprite)
//{
//
//
//}
//
//void NPC::updateNPC(float elapsedTime, float friction)
//{
//	if (CHARACTER_TYPE != GLC::STATIC_NPC) {
//		moveNPC(elapsedTime, friction);
//		animateNPC(elapsedTime);
//	}
//}
//
//void NPC::moveNPC(float elapsedTime, float friction)
//{
//
//	//Use elapsed time to calculate acceleraton between ticks to keep it consistant even if running slowly
//	this->xAcceleration = (elapsedTime / 1000);
//	this->yAcceleration = (elapsedTime / 1000);
//
//	//Apply friction on deceleration so the player slows faster when stoping
//	if (moveDirectionX == 0)
//		this->xAcceleration *= friction;
//	if (moveDirectionY == 0)
//		this->yAcceleration *= friction;
//
//	/*
//		- xSpeed / ySpeed is current speed of the player on the x / y axis.
//
//		- moveDirection is the direction the player is moving on the current axis,
//		  1 is forward, -1 is back, and 0 is stopped.
//
//		- maxSpeed is a constant determining the fastest the player can travel.
//
//		- Acceleration is a number usually between 0 and 1, representing the rate of acceletation and decelleration,
//		  1 is instant, 0 is never move.
//
//	*/
//
//	this->xSpeed += ((this->moveDirectionX * this->maxSpeed) - this->xSpeed) * this->xAcceleration;
//	this->ySpeed += ((this->moveDirectionY * this->maxSpeed) - this->ySpeed) * this->yAcceleration;
//
//}
//
//void NPC::animateNPC(float elapsedTime)
//{
//
//}
//void NPC::addTexture(sf::Texture t)
//{
//	this->tList.push_back(t);
//}