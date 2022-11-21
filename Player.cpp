//#include "Player.h"
//
//Player::Player()
//{
//	this->init();
//}
//
//Player::~Player()
//{
//	
//}
//
////Initialize variables
//void Player::init()
//{
//	this->setScale(sf::Vector2f(0.9, 0.9));
//	this->xPos = 10;
//	this->yPos = 10;
//	this->xSpeed = 0;
//	this->ySpeed = 0;
//	this->xAcceleration = 0.5;
//	this->yAcceleration = 0.5;
//	this->maxSpeed = 5;
//	this->frameDelay = 6;
//	this->frameCounter = 0;
//	this->moveUP = false;
//	this->moveLEFT = false;
//	this->moveDOWN = false;
//	this->moveRIGHT = false;
//}
////Set the starting location and default texture
//void Player::spawnPlayer(const sf::Vector2f initLocation)
//{
//	this->xPos = initLocation.x;
//	this->yPos = initLocation.y;
//	this->setPosition(initLocation);
//
//	curTexture = 0;
//	if(tList.size() > 0){
//
//		this->setTexture(tList[curTexture]);
//		this->setTextureRect(spriteRects[curTexture]);
//		this->isVisible = true;
//	}
//
//	boxSize.x = 28 * this->getScale().x;
//	boxSize.y = 50 * this->getScale().y;
//	
//}
//int Player::getLayer()
//{
//	return this->currentLayer;
//}
//void Player::setLayer(int layer)
//{
//	this->currentLayer = layer;
//}
////Update the players position, do movement code, and animate them
//void Player::updatePlayer(float elapsedTime, float friction)
//{	
//	if (!canMoveX) {
//		this->xSpeed = 0;
//	}	
//	if (!canMoveY) {
//		this->ySpeed = 0;
//	}
//	
//	this->xPos += this->xSpeed;
//	this->yPos += this->ySpeed;
//	this->setPosition(sf::Vector2f(this->xPos, this->yPos));
//	
//
//	movePlayer(elapsedTime, friction);
//	animatePlayer(elapsedTime);
//	
//	this->nextPosition = sf::Vector2f(this->xPos + this->xSpeed, this->yPos + this->ySpeed);
//	this->playerBoundingBox.setPosition(this->nextPosition.x, this->nextPosition.y);
//	this->playerBoundingBox.setSize(sf::Vector2(boxSize.x, boxSize.y));
//	
//}
////Read the user input and set the player direction to the corresponding key while it is pressed
//void Player::readInput(Input input)
//{
//	if (input.IsKeyPressed(Input::Key::Left)) {
//		moveLEFT = true;
//		unStop();
//		this->moveDirectionX = -1;
//	}
//	else if (input.IsKeyPressed(Input::Key::Right))
//	{
//		moveRIGHT = true;
//		unStop();
//		this->moveDirectionX = 1;
//	}
//	if (input.IsKeyPressed(Input::Key::Up))
//	{
//		moveUP = true;
//		unStop();
//		this->moveDirectionY = -1;
//	}
//	else if (input.IsKeyPressed(Input::Key::Down))
//	{
//		moveDOWN = true;
//		unStop();
//		this->moveDirectionY = 1;
//	}
//	if (input.IsKeyUp(Input::Key::Left)) {
//		moveLEFT = false;
//		if (moveDirectionX == -1)
//			stoppedLEFT = true;
//		this->moveDirectionX = 0;
//		if (moveRIGHT)
//			this->moveDirectionX = 1;
//	}
//	else if (input.IsKeyUp(Input::Key::Right))
//	{
//		moveRIGHT = false;
//		if (moveDirectionX == 1)
//			stoppedRIGHT = true;
//		this->moveDirectionX = 0;
//		if (moveLEFT)
//			this->moveDirectionX = -1;
//	}
//	if (input.IsKeyUp(Input::Key::Up))
//	{
//		moveUP = false;
//		if (moveDirectionY == -1) //Only stop if they were going up before
//			this->moveDirectionY = 0;
//		stoppedUP = true;
//		if (moveDOWN) //If the down key is still being pressed, go that direction,
//			this->moveDirectionY = 1;
//	}
//	else if (input.IsKeyUp(Input::Key::Down))
//	{
//		moveDOWN = false;
//		if (moveDirectionY == 1)
//			stoppedDOWN = true;
//		this->moveDirectionY = 0;
//		if (moveUP)
//			this->moveDirectionY = -1;
//	}
//}
//
////Move the player based on what keys are pressed
//void Player::movePlayer(float elapsedTime, float friction)
//{
//	//Use elapsed time to calculate acceleraton between ticks to keep it consistant even if running slowly
//	this->xAcceleration = (elapsedTime);
//	this->yAcceleration = (elapsedTime);
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
//}
////Animate the character based on what direction it is moving
//void Player::animatePlayer(float elapsedTime)
//{
//	if (frameCounter >= frameDelay) {
//		frameCounter = 0;
//		
//		switch (moveDirectionX) {
//
//		case 1:
//			curTexture = (curTexture >= 17 || curTexture < 10) ? 10 : curTexture + 1;
//			this->setTextureRect(mirrorRect(spriteRects[curTexture]));
//			break;
//		case -1:
//			curTexture = (curTexture >= 17 || curTexture < 10) ? 10 : curTexture + 1;
//			this->setTextureRect(spriteRects[curTexture]);
//			break;
//		case 0:
//			switch (moveDirectionY) {
//
//			case 1:
//				curTexture = (curTexture >= 8 || curTexture < 1) ? 1 : curTexture + 1;
//				this->setTextureRect(spriteRects[curTexture]);
//				break;
//			case -1:
//				curTexture = (curTexture >= 26 || curTexture < 19) ? 19 : curTexture + 1;
//				this->setTextureRect(spriteRects[curTexture]);
//				break;
//			case 0:
//				curTexture = stoppedDOWN ? 0 : curTexture;
//				curTexture = stoppedUP ? 18 : curTexture;
//				curTexture = stoppedLEFT ? 9 : curTexture;
//				this->setTextureRect(spriteRects[curTexture]);
//
//				if (stoppedRIGHT) {
//					curTexture = 9;
//					this->setTextureRect(mirrorRect(spriteRects[curTexture]));
//				}
//				break;
//			}
//		}
//		
//	}
//	else {
//
//		frameCounter += 1;
//	}
//	
//	
//	
//	
//}
//
//
////Hard code the dimensions for the sprite and add the frames to a list of rectangles
//void Player::addTexture(sf::Texture t)
//{
//	//Down Frames
//	spriteRects.push_back(sf::IntRect(0, 0, 28, 50));//0 Not moving
//	spriteRects.push_back(sf::IntRect(28, 0, 28, 50));//1
//	spriteRects.push_back(sf::IntRect(56, 0, 28, 50));//2 
//	spriteRects.push_back(sf::IntRect(84, 0, 25, 48));//3
//	spriteRects.push_back(sf::IntRect(109, 0, 28, 50));//4
//	spriteRects.push_back(sf::IntRect(137, 0, 28, 50));//5
//	spriteRects.push_back(sf::IntRect(165, 0, 27, 50));//6
//	spriteRects.push_back(sf::IntRect(192, 0, 25, 48));//7
//	spriteRects.push_back(sf::IntRect(217, 0, 28, 50));//8
//
//	//Side frames
//	spriteRects.push_back(sf::IntRect(245, 0, 25, 49));//9 Not moving
//	spriteRects.push_back(sf::IntRect(270, 0, 26, 47));//10
//	spriteRects.push_back(sf::IntRect(296, 0, 25, 47));//11
//	spriteRects.push_back(sf::IntRect(321, 0, 24, 47));//12
//	spriteRects.push_back(sf::IntRect(345, 0, 25, 48));//13
//	spriteRects.push_back(sf::IntRect(370, 0, 27, 47));//14
//	spriteRects.push_back(sf::IntRect(397, 0, 25, 47));//15
//	spriteRects.push_back(sf::IntRect(422, 0, 24, 48));//16
//	spriteRects.push_back(sf::IntRect(446, 0, 24, 48));//17
//
//	//Up frames
//	spriteRects.push_back(sf::IntRect(470, 0, 28, 47));//18 Not moving
//	spriteRects.push_back(sf::IntRect(498, 0, 28, 47));//19
//	spriteRects.push_back(sf::IntRect(526, 0, 26, 48));//20
//	spriteRects.push_back(sf::IntRect(552, 0, 26, 48));//21
//	spriteRects.push_back(sf::IntRect(578, 0, 28, 48));//22
//	spriteRects.push_back(sf::IntRect(606, 0, 28, 47));//23
//	spriteRects.push_back(sf::IntRect(634, 0, 26, 48));//24
//	spriteRects.push_back(sf::IntRect(660, 0, 26, 48));//25
//	spriteRects.push_back(sf::IntRect(686, 0, 28, 48));//26
//
//
//	this->tList.push_back(t);
//
//	boxSize.x = 28 * this->getScale().x;
//	boxSize.y = 50 * this->getScale().y;
//
//}
//
////Mirror rectangles for left / right movement
//sf::IntRect Player::mirrorRect(sf::IntRect rectToMirror)
//{
//	sf::IntRect mirroredRect;
//
//	mirroredRect.left = rectToMirror.left + rectToMirror.width;
//	mirroredRect.top = rectToMirror.top;
//	mirroredRect.width = -rectToMirror.width;
//	mirroredRect.height = rectToMirror.height;
//
//	return mirroredRect;
//}
//
////Call this to reset which direction the player last stopped at
//void Player::unStop()
//{
//	stoppedUP = false;
//	stoppedRIGHT = false;
//	stoppedDOWN = false;
//	stoppedLEFT = false;
//}
