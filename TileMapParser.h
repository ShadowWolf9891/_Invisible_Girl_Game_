#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <sstream>
#include "Tile.h"
#include "tileson.hpp"
#include "Object.h"
#include "C_Sprite.h"
#include "C_BoxCollider.h"

struct TileSheetData
{
	// The texture id will be retrieved by using our texture allocator.
	int textureId; // The id of the tile sets texture. 
	sf::Vector2u imageSize; // The size of the texture.
	int columns; // How many columns in the tile sheet.
	int rows; // How many rows in the tile sheet.
	sf::Vector2u tileSize; // The size of an individual tile.
	std::string filepath;
};
struct MapObjectData {
	int objectId;
	sf::Vector2i position;
	sf::Vector2u objectSize;
	sf::Vector2i origin;
	float rotation;
	std::string name;
};
struct MapObject {
	std::shared_ptr<MapObjectData> properties;

};
struct Layer
{
	std::vector<std::shared_ptr<Tile>> tiles;
	std::vector<std::shared_ptr<MapObject>> mapObjects;
	bool isVisible;
	int layerNum;
};


using MapTiles = std::vector< std::pair<std::string, std::shared_ptr<Layer>>>; // Stores layer names with layer.
using TileSet = std::unordered_map<unsigned int, std::shared_ptr<TileInfo>>; // Stores the different tile types that can be used.
using ObjectSet = std::unordered_map<unsigned int, std::shared_ptr<MapObjectData>>;
using TileSheets = std::map<int, std::shared_ptr<TileSheetData>>;

class TileMapParser
{
public:
	TileMapParser(ResourceAllocator<sf::Texture>& textureAllocator, SharedContext& context);
	std::vector<std::shared_ptr<Object>> Parse(const std::string& filename, std::unique_ptr<tson::IDecompressor<std::vector<uint8_t>, std::vector<uint8_t>>> decompressor = nullptr);

	

private:
	
	std::shared_ptr<TileSheets> BuildTileSheetData(std::shared_ptr<tson::Map> map);
	std::shared_ptr<MapTiles> BuildMapTiles(std::shared_ptr<tson::Map> map);

	std::pair<std::string, std::shared_ptr<Layer>>BuildLayer(tson::Layer& layer, std::shared_ptr<TileSheets> tileSheets);
	std::pair<std::string, std::shared_ptr<Layer>> BuildTileLayer(tson::Layer& layer, std::shared_ptr<TileSheets> tileSheets);
	std::pair<std::string, std::shared_ptr<Layer>> BuildObjectLayer(tson::Layer& layer, std::shared_ptr<TileSheets> tileSheets);
	//std::pair<std::string, std::shared_ptr<Layer>> BuildImageLayer(tson::Layer& layer, std::vector<std::shared_ptr<TileSheetData>> tileSheetData);
	
	SharedContext& context;
	ResourceAllocator<sf::Texture>& textureAllocator;
};