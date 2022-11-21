#include "TileMapParser.h"
TileMapParser::TileMapParser(ResourceAllocator<sf::Texture>& textureAllocator, SharedContext& context) : textureAllocator(textureAllocator), context(context)
{

}
std::vector<std::shared_ptr<Object>> TileMapParser::Parse(const std::string& filename, std::unique_ptr<tson::IDecompressor<std::vector<uint8_t>, std::vector<uint8_t>>> decompressor)
{
    tson::Tileson t;
    tson::Project m_project;
    std::vector<std::shared_ptr<Object>> tileObjects;

    std::shared_ptr<tson::Map> map = t.parse(fs::path(filename));

    if (map->getStatus() == tson::ParseStatus::OK)
    {
        std::shared_ptr<MapTiles> tiles = BuildMapTiles(map);

        for (const auto& layer : *tiles)
        {
            if (!layer.second->tiles.empty()) {

                for (const auto& tile : layer.second->tiles)
                {

                    std::shared_ptr<TileInfo> tileInfo = tile->properties;
                    std::shared_ptr<Object> tileObject = std::make_shared<Object>(&context);

                    //TODO: tile scale should be set at the data level.
                    const unsigned int tileScale = 1;

                    if (layer.second->isVisible)
                    {
                        auto sprite = tileObject->AddComponent<C_Sprite>();
                        sprite->Load(tileInfo->textureID);
                        sprite->SetTextureRect(tileInfo->textureRect);
                        sprite->SetSortOrder(layer.second->layerNum);
                        // Set the tiles layer to background for now
                        sprite->SetDrawLayer(DrawLayer::Background);
                    }
                    //sprite->SetScale(tileScale, tileScale);
                        // Calculate world position.
                    
                    
                    float x = tile->x * tileScale;
                    float y = tile->y * tileScale;
                    tileObject->transform->SetPosition(x, y);

                    // Add new tile Object to the collection.
                    tileObjects.emplace_back(tileObject);

                }
            }
            else if(!layer.second->mapObjects.empty()) {

                for (const auto& obj : layer.second->mapObjects)
                {
                    std::shared_ptr<Object> cObject = std::make_shared<Object>(&context);
                    std::shared_ptr<MapObjectData> mapObjectInfo = obj->properties;

                    const unsigned int objScale = 1;
                    float x = obj->properties->position.x * objScale;
                    float y = obj->properties->position.y * objScale;

                    auto collider = cObject->AddComponent<C_BoxCollider>();
                    float left = x - (mapObjectInfo->objectSize.x * objScale) * 0.5f;
                    float top = y - (mapObjectInfo->objectSize.y * objScale) * 0.5f;
                    float width = mapObjectInfo->objectSize.x * objScale;
                    float height = mapObjectInfo->objectSize.y * objScale;
                    collider->SetCollidable(sf::FloatRect(sf::Vector2f(left, top), sf::Vector2f(width, height)));
                    collider->SetLayer(CollisionLayer::Tile);

                    cObject->transform->SetPosition(x - 16.f, y - 16.f);
                    tileObjects.emplace_back(cObject);
                }
            }
        }
    }
    else
    { 
        std::cout << "Parse error: " << map->getStatusMessage() << std::endl;
    }

    return tileObjects;

}

/*!
 * Stores and loads the image if it doesn't exists, and retrieves it if it does.
 * @param image
 * @return
 */
std::shared_ptr<TileSheets> TileMapParser::BuildTileSheetData(std::shared_ptr<tson::Map> map)
{
    TileSheets tileSheets;
    for (auto& tileSheet : map->getTilesets()) {

        TileSheetData tileSheetData;
        int firstid = tileSheet.getFirstgid();
        tileSheetData.textureId = textureAllocator.Add(tileSheet.getImagePath().string());
        tileSheetData.tileSize.x = tileSheet.getTileSize().x;
        tileSheetData.tileSize.y = tileSheet.getTileSize().y;
        tileSheetData.columns = tileSheet.getColumns();
        tileSheetData.rows = tileSheet.getTiles().size() / tileSheetData.columns;
        tileSheetData.imageSize.x = tileSheet.getImageSize().x;
        tileSheetData.imageSize.y = tileSheet.getImageSize().y;

        std::cout << "Stored " << tileSheet.getImagePath().string() << " as texture in memory." << std::endl;
        
        tileSheets.insert( std::make_pair(firstid, std::make_shared<TileSheetData>(tileSheetData)) );
                
    }
    
    return std::make_shared<TileSheets>(tileSheets);
}

std::shared_ptr<MapTiles> TileMapParser::BuildMapTiles(std::shared_ptr<tson::Map> map)
{
    
    std::shared_ptr<MapTiles> mapTiles = std::make_shared<MapTiles>();

    std::shared_ptr<TileSheets> tileSheets = BuildTileSheetData(map);
    

    for (auto& layer : map->getLayers())
    {
        std::pair<std::string, std::shared_ptr<Layer>> mapLayer = BuildLayer(layer, tileSheets);
        mapTiles->emplace_back(mapLayer);
    }

    return mapTiles;

}

std::pair<std::string, std::shared_ptr<Layer>> TileMapParser::BuildLayer(tson::Layer& layer, std::shared_ptr<TileSheets> tileSheets)
{
    switch (layer.getType())
    {
    case tson::LayerType::TileLayer:
        return BuildTileLayer(layer, tileSheets);
        break;
    case tson::LayerType::ObjectGroup:
        return BuildObjectLayer(layer, tileSheets);
        break;

    case tson::LayerType::ImageLayer:
        //return BuildImageLayer(layer, tileSheetData);
        break;

    case tson::LayerType::Group:
        //There are no group layers in the demo map, but it basicly just contains sub layers
        //You can call this function on those layers, like this:
        for (auto& l : layer.getLayers())
            BuildLayer(l, tileSheets);

        break;

    default:
        break;
    }

    //return BuildTileLayer(layer, tileSheets);
}
std::pair<std::string, std::shared_ptr<Layer>> TileMapParser::BuildTileLayer(tson::Layer& layer, std::shared_ptr<TileSheets> tileSheets)//, tson::Tileset* tileset)
{

    TileSet tileList;
    std::shared_ptr<TileSheetData> tileSheet;

    std::shared_ptr<Layer> layerObj = std::make_shared<Layer>();

    if (layer.getProperties().getProperty("LayerOrder")->getValue().has_value()) {
        layerObj->layerNum = layer.get<int>("LayerOrder");
    }
    //pos = position in tile units
    for (auto& [pos, tileObject] : layer.getTileObjects()) //Loops through absolutely all existing tiles
    {
        int tileID = tileObject.getTile()->getGid();
        if (tileID != 0) {

            auto itr = tileList.find(tileID);
            if (itr == tileList.end())
            {
                int firstId = 0;
                for (auto iter = tileSheets->rbegin(); iter != tileSheets->rend(); ++iter)
                {
                    if (tileID >= iter->first)
                    {
                        // Set firstid
                        firstId = iter->first;
                        tileSheet = iter->second;
                        break;
                    }
                }
                int textureX = (tileID - firstId) % tileSheet->columns;
                int textureY = (tileID - firstId) / tileSheet->columns;
               // tson::Tileset* tileset = tileObject.getTile()->getTileset();
                //int tileSetID = textureAllocator.Add(tileset->getImagePath().string());
                std::shared_ptr<TileInfo> tileInfo = std::make_shared<TileInfo>(tileSheet->textureId, tileID,
                    sf::IntRect(sf::Vector2i(textureX * tileSheet->tileSize.x, textureY * tileSheet->tileSize.y),
                        sf::Vector2i(tileSheet->tileSize.x,tileSheet->tileSize.y)));
                itr = tileList.insert(std::make_pair(tileID, tileInfo)).first;
            }

            std::shared_ptr<Tile> tile = std::make_shared<Tile>();
            tile->properties = itr->second;

            tile->x = tileObject.getPosition().x;
            tile->y = tileObject.getPosition().y;
            tile->z = 0; //layer.get<int>("LayerOrder");  //Obsolete, replaced with layerObj->layerNum. Use instead of sort order within layer
            
            layerObj->tiles.emplace_back(tile);
        }
       
    }
    const std::string layerName = layer.getName();

    layerObj->isVisible = layer.isVisible();
   
    return std::make_pair(layerName, layerObj);

}
//std::pair<std::string, std::shared_ptr<Layer>> TileMapParser::BuildImageLayer(tson::Layer& layer, std::vector<std::shared_ptr<TileSheetData>> tileSheetData)
//{
//    /*sf::Sprite* sprite = storeAndLoadImage(layer.getImage(), { layer.getOffset().x, layer.getOffset().y });
//    if (sprite != nullptr)
//        this->window->draw(*sprite);*/
//}
std::pair<std::string, std::shared_ptr<Layer>> TileMapParser::BuildObjectLayer(tson::Layer& layer, std::shared_ptr<TileSheets> tileSheets)
{
    //tson::Tileset* tileset = m_map->getTileset("demo-tileset");

    ObjectSet objectList;
    std::shared_ptr<Layer> layerObj = std::make_shared<Layer>();

    if (layer.getProperties().getProperty("LayerOrder")->getValue().has_value()) {
        layerObj->layerNum = layer.get<int>("LayerOrder");
    }

    for (auto& obj : layer.getObjects())
    {
        switch (obj.getObjectType())
        {
        case tson::ObjectType::Object:
        {
            int objectId = obj.getId();
            if (objectId != 0) {
                auto itr = objectList.find(objectId);
                if (itr == objectList.end())
                {
                    sf::Vector2u objectSize = sf::Vector2u(obj.getSize().x, obj.getSize().y);
                    sf::Vector2i origin = sf::Vector2i(objectSize.x / 2, objectSize.y / 2);
                    sf::Vector2i position = sf::Vector2i(obj.getPosition().x, obj.getPosition().y);
                    float rotation = obj.getRotation();
                    std::string name = obj.getName();

                    std::shared_ptr<MapObjectData> objectData = std::make_shared<MapObjectData>(objectId, position, objectSize, origin, rotation, name);

                    itr = objectList.insert(std::make_pair(objectId, objectData)).first;
                }
                std::shared_ptr<MapObject> mapObject = std::make_shared<MapObject>();
                mapObject->properties = itr->second;

                layerObj->mapObjects.emplace_back(mapObject);
            }
            break;
        }


        case tson::ObjectType::Ellipse:
            //Not used by the demo map, but you could use the properties of obj for a sf::CircleShape
            break;

        case tson::ObjectType::Rectangle:
        {
            int objectId = obj.getId();

            if (objectId != 0) {
                auto itr = objectList.find(objectId);
                if (itr == objectList.end())
                {
                    sf::Vector2u objectSize = sf::Vector2u(obj.getSize().x, obj.getSize().y);
                    sf::Vector2i origin = sf::Vector2i(objectSize.x / 2, objectSize.y / 2);
                    sf::Vector2i position = sf::Vector2i(obj.getPosition().x + origin.x, obj.getPosition().y + origin.y);
                    float rotation = obj.getRotation();
                    std::string name = obj.getName();

                    std::shared_ptr<MapObjectData> objectData = std::make_shared<MapObjectData>(objectId, position, objectSize, origin, rotation, name);

                    itr = objectList.insert(std::make_pair(objectId, objectData)).first;
                }

                std::shared_ptr<MapObject> mapObject = std::make_shared<MapObject>();
                mapObject->properties = itr->second;

                layerObj->mapObjects.emplace_back(mapObject);
            }
            break;
        }
        case tson::ObjectType::Point:
            //Not used by the demo map but one could use the points of obj (polygon or polyline)
            //then pass them into logic like this:
            //sf::Vertex line[] =
            //        {
            //                sf::Vertex(sf::Vector2f(obj.getPolylines()[0].x, obj.getPolylines()[0].y)),
            //                sf::Vertex(sf::Vector2f(obj.getPolylines()[1].x, obj.getPolylines()[1].y))
            //        };
            //m_window.draw(line, 2, sf::Lines);
            break;

        case tson::ObjectType::Polygon:
            //Not used by the demo map, but you could use the properties of obj for a sf::ConvexShape
            break;

        case tson::ObjectType::Polyline:
            //Not used by the demo map, but you could use the properties of obj for a sf::ConvexShape
            break;

            //case tson::ObjectType::Text:
            //    m_demoText.setFont(m_font);
            //    m_demoText.setPosition({ (float)obj.getPosition().x, (float)obj.getPosition().y });
            //    m_demoText.setString(obj.getText().text);
            //    m_demoText.setCharacterSize(32); //It is 16, but makes it double for a "sharp text hack"
            //    m_demoText.setScale(0.5f, 0.5f); //Half scale for making a sharp text.
            //    m_window.draw(m_demoText);
            //    break;

        case tson::ObjectType::Template:
            //use obj.getTemplate() to get the connected template. References an external file not covered by Tileson.
            //obj.getPosition() and obj.getId() should also be related to the placement of the template.

            break;

        default:
            break;
        }
    }

    const std::string layerName = layer.getName();

    layerObj->isVisible = layer.isVisible();

    return std::make_pair(layerName, layerObj);
}