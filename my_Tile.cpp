#include "my_Tile.h"

my_Tile::my_Tile()
{


}

my_Tile::my_Tile(int textureID, unsigned int tileID, sf::IntRect textureRect)
{


}

my_Tile::~my_Tile()
{


}

int my_Tile::getLayer()
{
	return currentLayer;
}

void my_Tile::setLayer(int layer)
{
	this->currentLayer = layer;
}

void my_Tile::setupDrawingRectangles(tson::Rect r)
{
	sf::IntRect rec;
	rec.left = r.x;
	rec.top = r.y;
	rec.width = r.width;
	rec.height = r.height;
	drawingRectangles.push_back(rec);

}

void my_Tile::updateTile(float elapsedTime)
{
	if (hasAnimation) {
		setTextureRect(drawingRectangles[spriteAnim->getCurrentFrameNumber()]);
		spriteAnim->update(elapsedTime / 50);
	}
	
}




