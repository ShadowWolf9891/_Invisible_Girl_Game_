#pragma once

#include "Invisible_Girl_Game.h"

class my_Tile : public sf::Sprite
{
private:

	int currentLayer = 0;

public:
	//Constructor / Deconstructor
	my_Tile();
	my_Tile(int textureID, unsigned int tileID, sf::IntRect textureRect) ;
	virtual ~my_Tile();

	//Variables
	uint32_t tileID;
	int textureID;
	sf::IntRect textureRect;

	tson::Animation* spriteAnim = nullptr;
	std::vector <sf::IntRect> drawingRectangles;
	bool hasAnimation = false;



	int getLayer(); //Gets the layer the tile is on
	void setLayer(int layer); //Sets the layer the tile is on. Should only happen once when loading from json file

	//Functions
	void setupDrawingRectangles(tson::Rect);
	void updateTile(float elapsedTime);
	

};

