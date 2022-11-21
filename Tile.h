#pragma once
#include "ResourceAllocator.h"
#include <SFML/Graphics.hpp>
// Stores common tile data.
struct TileInfo
{
	TileInfo() : tileID(-1) //Constructor for no tileInfo
	{

	}
	TileInfo(int textureID, unsigned int tileID, sf::IntRect textureRect) : textureID(textureID), tileID(tileID), textureRect(textureRect)  //Constructor for given info
	{

	}
	int tileID;
	int textureID;
	sf::IntRect textureRect;
	
};
struct Tile
{
	std::shared_ptr<TileInfo> properties;
	int x; //X position within tile grid
	int y; //Y position within tile grid
	int z; //The sort order of a tile on a specific layer
};