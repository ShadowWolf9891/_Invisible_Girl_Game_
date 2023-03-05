#include "Game.h"

//Constructors / Deconstructors

Game::Game() : window("Invisible Girl Game")
{
	this->initVariables();
	//this->initWindow();
    //this->initTextures();
   // this->initPlayer();
    //this->initEnemies();
    //this->loadLevel(GLC::HARBOR_VILLAGE);
    //this->drawMap();
}

Game::~Game()
{
	
}

//Accessors

/*
    Checks to see if the window is open

    @return bool

*/
bool Game::IsRunning() const
{
	return window.IsOpen();
}


//Private Functions

/*
    Initializes the defaults for variables.

    -Set window to be a null pointer

    @return void

*/
void Game::initVariables()
{   
    std::shared_ptr<SceneSplashScreen> splashScreen = std::make_shared<SceneSplashScreen>(workingDir, textureAllocator, window, fontAllocator); //Smart pointer to splash screen scene
    std::shared_ptr<SceneMainMenu> mainMenuScreen = std::make_shared<SceneMainMenu>(workingDir, textureAllocator, window, fontAllocator);
    std::shared_ptr<SceneGame> gameScene = std::make_shared<SceneGame>(workingDir, textureAllocator, window, fontAllocator); //Smart pointer to game scene

    //Add scenes to scenestatemachine and return the ID's
    unsigned int splashScreenID = sceneStateMachine.Add(splashScreen); 
    unsigned int gameSceneID = sceneStateMachine.Add(gameScene);
    unsigned int mainMenuID = sceneStateMachine.Add(mainMenuScreen);

    splashScreen->SetSwitchToScene(mainMenuID); //Set splashscreen to transition to gamescreen when switched to
    mainMenuScreen->SetSwitchToScene(gameSceneID);
    sceneStateMachine.SwitchTo(splashScreenID); //Set game to start on splashscreen

	//this->window = nullptr;
    deltaTime = clock.restart().asSeconds();
    
}


/*
    Initializes the game window.

    -Set videomode to specific size
    -Create window

    @return void

*/
//void Game::initWindow()
//{
//	this->videoMode.height = GLC::W_HEIGHT;
//	this->videoMode.width = GLC::W_WIDTH;
//	this->window = new sf::RenderWindow(this->videoMode, "2D Game", sf::Style::Titlebar | sf::Style::Close);
//    this->window->setFramerateLimit(60);
//
//    this->mainView.reset(sf::FloatRect(playerSpawn.x, playerSpawn.y, GLC::W_WIDTH, GLC::W_HEIGHT));
//    this->mainView.zoom(0.5);
//    this->mainView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
//
//    this->window->setView(this->mainView);
//
//    /*this->begin = std::chrono::steady_clock::now();
//    this->end = std::chrono::steady_clock::now();*/
//
//}

//void Game::initTextures()
//{
//    jsonMapFiles[GLC::HARBOR_VILLAGE] = "../level_data/Town_1_data.json"; 
//    
//    //pTextureName = "../Assets/Player/MC_Sprite_Sheet.png"; //Replace with json file loading
//
//}


//void Game::initEnemies()
//{
//    
//}
//int Game::initPlayer()
//{
//    if (!pTexture.loadFromFile(pTextureName)) {
//        std::cout << "Failed to load Player \n";
//        return EXIT_FAILURE;
//    }
//    this->player.isVisible = true;
//    this->player.addTexture(pTexture);
//    this->playerPreviousLocation = this->playerSpawn;
//   
//    return 0;
//
//}
//void Game::loadLevel(uint32_t levelName)
//{
//    if (!jsonMapFiles[levelName].empty()) {
//        m_mapIndex = levelName;
//        m_projectMaps[jsonMapFiles[m_mapIndex]] = parseMap(jsonMapFiles[m_mapIndex]);
//    }
//       
//    //this->player.spawnPlayer(playerSpawn);
//
//}
//std::unique_ptr<tson::Map> Game::parseMap(const std::string& filename, std::unique_ptr<tson::IDecompressor<std::vector<uint8_t>, std::vector<uint8_t>>> decompressor)
//{
//    tson::Tileson t;
//    std::unique_ptr<tson::Map> map = t.parse(fs::path(m_basePath / filename), std::move(decompressor));
//
//    if (map->getStatus() == tson::ParseStatus::OK)
//    {
//        for (auto& tileset : map->getTilesets())
//        {
//            fs::path tilesetPath = getTilesetImagePath(tileset); //tileset.getImage().u8string()
//            storeAndLoadImage(tilesetPath.string(), { 0, 0 });
//        }
//
//        return std::move(map);
//    }
//    else
//        std::cout << "Parse error: " << map->getStatusMessage() << std::endl;
//
//    return nullptr;
//}
//bool Game::parseProject(const std::string& filename)
//{
//    using namespace std::string_literals; //Used for s-suffix
//    tson::Tileson t;
//    fs::path projectBase = fs::path(m_basePath);
//    int projectCount{ 0 };
//    int worldCount{ 0 };
//    if (m_project.parse(fs::path(projectBase / filename)))
//    {
//        for (const auto& folder : m_project.getFolders())
//        {
//            if (folder.hasWorldFile())
//            {
//                const auto& world = folder.getWorld();
//                for (const auto& data : world.getMapData())
//                {
//                    std::unique_ptr<tson::Map> map = t.parse(fs::path(world.getFolder() / data.fileName));
//                    if (map->getStatus() == tson::ParseStatus::OK)
//                    {
//                        ++worldCount;
//                        for (auto& tileset : map->getTilesets())
//                        {
//                            fs::path tilesetPath = getTilesetImagePath(tileset); //fs::path(fs::path("../") / tileset.getImage().filename().u8string());
//                            storeAndLoadImage(tilesetPath.string(), { 0, 0 });
//                        }
//
//                        m_worldMaps.push_back(std::move(map));
//                        m_worldData.emplace_back(data);
//                        m_worldVisibilityFlags.push_back(true);
//                        std::string info = "Part of .world file (inside a project) '"s + world.getPath().filename().string() + "': " + data.fileName;
//                        m_worldMapInfo.emplace_back(info);
//                    }
//                    else
//                    {
//                        std::cout << "Parse error: " << map->getStatusMessage() << std::endl;
//                        return false;
//                    }
//                }
//            }
//            else
//            {
//                for (const auto& file : folder.getFiles())
//                {
//                    std::unique_ptr<tson::Map> map = t.parse(fs::path(folder.getPath() / file.filename()));
//                    if (map->getStatus() == tson::ParseStatus::OK)
//                    {
//                        ++projectCount;
//
//                        for (auto& tileset : map->getTilesets())
//                        {
//                            fs::path tilesetPath = getTilesetImagePath(tileset); //fs::path(fs::path("../") / tileset.getImage().filename().u8string());
//                            storeAndLoadImage(tilesetPath.string(), { 0, 0 });
//                        }
//                        m_projectMaps[file.filename().string()] = std::move(map);
//                        std::string info = "Part of project file '"s + m_project.getPath().filename().string() + "': " + file.filename().string();
//                        m_projectMapInfo.emplace_back(info);
//                    }
//                    else
//                    {
//                        std::cout << "Parse error: " << map->getStatusMessage() << std::endl;
//                        return false;
//                    }
//                }
//            }
//        }
//    }
//    else
//    {
//        std::cout << "Project parse error! " << std::endl;
//        return false;
//    }
//
//    return true;
//}
//
//void Game::drawMap()
//{
//    m_positionOffset = { 0,0 };
//    m_currentInfo = (m_mapIndex == 0) ? "This is just a regular Tiled json-map" : "";
//
//    m_currentMap = m_projectMaps[jsonMapFiles[m_mapIndex]].get();
//    //m_currentInfo = m_projectMapInfo.at(m_mapIndex); //If I need to specify map info, change it to a map instead of vector
//
//    if (m_mapIndex <= m_maxMapIndex)
//    {
//        if (m_currentMap != nullptr)
//        {
//            for (auto& layer : m_currentMap->getLayers())
//                drawLayer(layer);
//        }
//    }
//    else //WORLD
//    {
//        for (int i = 0; i < m_worldMaps.size(); ++i)
//        {
//            m_currentMap = m_worldMaps.at(i).get();
//            tson::Vector2i tileSize = m_currentMap->getTileSize();
//
//            const tson::WorldMapData& data = m_worldData.at(i);
//            m_positionOffset = { (data.position.x + data.size.x), (data.position.y + data.size.y) };
//            bool isVisible = m_worldVisibilityFlags[i];
//            if (isVisible)
//            {
//                for (auto& layer : m_currentMap->getLayers())
//                    drawLayer(layer);
//            }
//
//        }
//    }
//}
//void Game::drawTileLayer(tson::Layer& layer)//, tson::Tileset* tileset)
//{
//    //pos = position in tile units
//    for (auto& [pos, tileObject] : layer.getTileObjects()) //Loops through absolutely all existing tiles
//    {
//        //Set sprite data to draw the tile
//        tson::Tileset* tileset = tileObject.getTile()->getTileset();
//        bool hasAnimation = tileObject.getTile()->getAnimation().any();
//        tson::Rect drawingRect;
//        
//        tson::Vector2f position = tileObject.getPosition();
//        position = { position.x + (float)m_positionOffset.x, position.y + (float)m_positionOffset.y };
//        //sf::Vector2f position = {(float)obj.getPosition().x + (float)m_positionOffset.x, (float)obj.getPosition().y + (float)m_positionOffset.y};
//        fs::path tilesetPath = getTilesetImagePath(*tileset);
//        Tile* sprite = storeAndLoadImage(tilesetPath.generic_string(), { 0, 0 });
//        
//        if (sprite != nullptr)
//        {
//            if (!hasAnimation) {
//                drawingRect = tileObject.getDrawingRect();
//                sprite->setTextureRect({ drawingRect.x, drawingRect.y, drawingRect.width, drawingRect.height });
//                
//            }
//            else
//            {
//
//                uint32_t ownerId = tileObject.getTile()->getId();
//                sprite->spriteAnim = &tileObject.getTile()->getAnimation();
//
//                for (int curFrame = 0; curFrame < tileObject.getTile()->getAnimation().size(); curFrame++) {
//                    sprite->spriteAnim->setCurrentFrame(curFrame);
//                    uint32_t tileId = tileObject.getTile()->getAnimation().getCurrentTileId();
//                    tson::Tile* animatedTile = tileset->getTile(tileset->getFirstgid() + tileId - 1);
//                    tson::Rect dRect = animatedTile->getDrawingRect();
//                    sprite->setupDrawingRectangles(dRect);
//                }
//                sprite->spriteAnim->setCurrentFrame(0);
//            }
//
//            sf::Vector2f scale = sprite->getScale();
//            sf::Vector2f originalScale = scale;
//            float rotation = sprite->getRotation();
//            float originalRotation = rotation;
//            sf::Vector2f origin{ ((float)drawingRect.width) / 2, ((float)drawingRect.height) / 2 };
//           /* BoundingBox bx; 
//            if (!tileObject.getTile()->getObjectgroup().getObjectsByType(tson::ObjectType::Polygon).empty()){
//
//               bx.addPoly(tileObject.getTile()->getObjectgroup().getObjectsByType(tson::ObjectType::Polygon).front());
//               
//            }*/
//            
//            if (tileObject.getTile()->hasFlipFlags(tson::TileFlipFlags::Horizontally))
//                scale.x = -scale.x;
//            if (tileObject.getTile()->hasFlipFlags(tson::TileFlipFlags::Vertically))
//                scale.y = -scale.y;
//            if (tileObject.getTile()->hasFlipFlags(tson::TileFlipFlags::Diagonally))
//                rotation += 90.f;
//
//            position = { position.x + origin.x, position.y + origin.y };
//            sprite->setOrigin(origin);
//            sprite->setPosition(position.x, position.y);
//
//            sprite->setScale(scale);
//            sprite->setRotation(rotation);
//            sprite->hasAnimation = hasAnimation;
//
//            sprite->tileID = tileObject.getTile()->getId();
//            sprite->setLayer(layer.getId());
//
//            Tile newTile = *sprite;
//            //this->tilesToDraw[newTile.getLayer()].emplace(sf::Vector2i(std::round(newTile.getPosition().x / newTile.getGlobalBounds().width), std::round(newTile.getPosition().y / newTile.getGlobalBounds().height)), newTile);
//
//
//            sprite->setScale(originalScale);       //Since we used a shared sprite for this example, we must reset the scale.
//            sprite->setRotation(originalRotation); //Since we used a shared sprite for this example, we must reset the rotation.
//            sprite->drawingRectangles.clear();
//            sprite->spriteAnim = nullptr;
//            sprite->hasAnimation = false;
//            sprite->tileID = NULL;
//        }
//    }
//}
//void Game::drawImageLayer(tson::Layer& layer)
//{
//    /*sf::Sprite* sprite = storeAndLoadImage(layer.getImage(), { layer.getOffset().x, layer.getOffset().y });
//    if (sprite != nullptr)
//        this->window->draw(*sprite);*/
//}
//void Game::drawObjectLayer(tson::Layer& layer)
//{
//    //tson::Tileset* tileset = m_map->getTileset("demo-tileset");
//    auto* map = layer.getMap();
//    for (auto& obj : layer.getObjects())
//    {
//        switch (obj.getObjectType())
//        {
//        case tson::ObjectType::Object:
//        {
//            tson::Tileset* tileset = layer.getMap()->getTilesetByGid(obj.getGid());
//            sf::Vector2f offset = getTileOffset(obj.getGid(), map, tileset);
//
//            sf::Sprite* sprite = storeAndLoadImage(getTilesetImagePath(*tileset).string(), { 0,0 });
//            std::string name = obj.getName();
//            sf::Vector2f position = { (float)obj.getPosition().x + (float)m_positionOffset.x, (float)obj.getPosition().y + (float)m_positionOffset.y };
//            if (sprite != nullptr)
//            {
//                sf::Vector2f scale = sprite->getScale();
//                sf::Vector2f originalScale = scale;
//                float rotation = sprite->getRotation();
//                float originalRotation = rotation;
//                sf::Vector2f origin{ ((float)m_map->getTileSize().x) / 2, ((float)map->getTileSize().y) / 2 };
//
//                if (obj.hasFlipFlags(tson::TileFlipFlags::Horizontally))
//                    scale.x = -scale.x;
//                if (obj.hasFlipFlags(tson::TileFlipFlags::Vertically))
//                    scale.y = -scale.y;
//                if (obj.hasFlipFlags(tson::TileFlipFlags::Diagonally))
//                    rotation += 90.f;
//
//                position = { position.x + origin.x, position.y + origin.y };
//                sprite->setOrigin(origin);
//
//                sprite->setTextureRect({ (int)offset.x, (int)offset.y, map->getTileSize().x, map->getTileSize().y });
//                sprite->setPosition({ position.x, position.y - map->getTileSize().y });
//
//                sprite->setScale(scale);
//                sprite->setRotation(rotation);
//
//                sprite->setScale(originalScale);       //Since we used a shared sprite for this example, we must reset the scale.
//                sprite->setRotation(originalRotation); //Since we used a shared sprite for this example, we must reset the rotation.
//            }
//            break;
//        }
//
//
//        case tson::ObjectType::Ellipse:
//            //Not used by the demo map, but you could use the properties of obj for a sf::CircleShape
//            break;
//
//        case tson::ObjectType::Rectangle:
//        {
//            sf::RectangleShape rs;
//            rs.setPosition(obj.getPosition().x, obj.getPosition().y);
//            rs.setSize(sf::Vector2f(obj.getSize().x, obj.getSize().y));
//
//            if (m_collisionBoxes[layer.getName()].size() == 0) {
//                std::vector<sf::RectangleShape> rList;
//                rList.emplace_back(rs);
//                m_collisionBoxes[layer.getName()] = rList;
//            }
//            else {
//
//                m_collisionBoxes[layer.getName()].push_back(rs);
//
//            }
//
//            break;
//        }
//        case tson::ObjectType::Point:
//            //Not used by the demo map but one could use the points of obj (polygon or polyline)
//            //then pass them into logic like this:
//            //sf::Vertex line[] =
//            //        {
//            //                sf::Vertex(sf::Vector2f(obj.getPolylines()[0].x, obj.getPolylines()[0].y)),
//            //                sf::Vertex(sf::Vector2f(obj.getPolylines()[1].x, obj.getPolylines()[1].y))
//            //        };
//            //m_window.draw(line, 2, sf::Lines);
//            break;
//
//        case tson::ObjectType::Polygon:
//            //Not used by the demo map, but you could use the properties of obj for a sf::ConvexShape
//            break;
//
//        case tson::ObjectType::Polyline:
//            //Not used by the demo map, but you could use the properties of obj for a sf::ConvexShape
//            break;
//
//        //case tson::ObjectType::Text:
//        //    m_demoText.setFont(m_font);
//        //    m_demoText.setPosition({ (float)obj.getPosition().x, (float)obj.getPosition().y });
//        //    m_demoText.setString(obj.getText().text);
//        //    m_demoText.setCharacterSize(32); //It is 16, but makes it double for a "sharp text hack"
//        //    m_demoText.setScale(0.5f, 0.5f); //Half scale for making a sharp text.
//        //    m_window.draw(m_demoText);
//        //    break;
//
//        case tson::ObjectType::Template:
//            //use obj.getTemplate() to get the connected template. References an external file not covered by Tileson.
//            //obj.getPosition() and obj.getId() should also be related to the placement of the template.
//
//            break;
//
//        default:
//            break;
//        }
//    }
//}
/*!
 * Stores and loads the image if it doesn't exists, and retrieves it if it does.
 * @param image
 * @return
 */
//Tile *Game::storeAndLoadImage(const std::string& image, const sf::Vector2f& position)
//{
//    if (m_textures.count(image) == 0)
//    {
//        fs::path path = m_basePath / image;
//        if (fs::exists(path) && fs::is_regular_file(path))
//        {
//            std::unique_ptr<sf::Texture> tex = std::make_unique<sf::Texture>();
//            bool imageFound = tex->loadFromFile(path.generic_string());
//            std::cout << "Stored " << path.generic_string() << " as texture in memory." << std::endl;
//            if (imageFound)
//            {
//                std::unique_ptr < my_Tile > spr = std::make_unique<my_Tile>();
//                spr->setTexture(*tex);
//                spr->setPosition(position);
//                m_textures[image] = std::move(tex);
//                m_sprites[image] = std::move(spr);
//            }
//        }
//        else
//            std::cout << "Could not find: " << path.generic_string() << std::endl;
//    }
//
//    if (m_sprites.count(image) > 0)
//        return m_sprites[image].get();
//
//    return nullptr;
//}
//void Game::updateAnimations(Tile& t)
//{
//        ////Time needs to be received as microseconds to get the right precision.
//        float ms = (float)((double)deltaTime / 1000);
//       
//       
//}
//sf::Vector2f Game::getTileOffset(int tileId, tson::Map* map, tson::Tileset* tileset)
//{
//
//    //tson::Tileset* tileset = map->getTileset("demo-tileset");
//
//    uint32_t firstId = tileset->getFirstgid(); //First tile id of the tileset
//    int columns = tileset->getColumns(); //For the demo map it is 8.
//    int rows = tileset->getTileCount() / columns;
//    uint32_t lastId = (tileset->getFirstgid() + tileset->getTileCount()) - 1;
//
//    //With this, I know that it's related to the tileset above (though I only have one tileset)
//    if (tileId >= firstId && tileId <= lastId)
//    {
//        uint32_t baseTilePosition = (tileId - firstId);
//
//        int tileModX = (baseTilePosition % columns);
//        int currentRow = (baseTilePosition / columns);
//        int offsetX = (tileModX != 0) ? ((tileModX)*map->getTileSize().x) : (0 * map->getTileSize().x);
//        int offsetY = (currentRow < rows - 1) ? (currentRow * map->getTileSize().y) : ((rows - 1) * map->getTileSize().y);
//        return sf::Vector2f((float)offsetX, (float)offsetY);
//    }
//
//    return { 0.f, 0.f };
//}
//void Game::drawLayer(tson::Layer& layer)
//{
//
//    switch (layer.getType())
//    {
//    case tson::LayerType::TileLayer:
//        drawTileLayer(layer);
//        break;
//
//    case tson::LayerType::ObjectGroup:
//        drawObjectLayer(layer);
//        break;
//
//    case tson::LayerType::ImageLayer:
//        drawImageLayer(layer);
//        break;
//
//    case tson::LayerType::Group:
//        //There are no group layers in the demo map, but it basicly just contains sub layers
//        //You can call this function on those layers, like this:
//        for (auto& l : layer.getLayers())
//            drawLayer(l);
//
//        break;
//
//    default:
//        break;
//    }
//}


//void Game::unloadLevel() {
//    //tilesToDraw.clear();
//
//}


/*
    Updates the parts that change of the game.

    -Check for events
    -Render screen

    @return void

*/
void Game::Update()
{
    window.Update();
    sceneStateMachine.Update(deltaTime);
}

void Game::LateUpdate()
{
    sceneStateMachine.LateUpdate(deltaTime);
}



//void Game::spawnEnemy()
//{
//    
//
//}

/*
    Reacts to events that have been triggered.

    -Loop through events while there are events to loop through
    -Switch / case to determine type of event
    -Respond to event
    -break loop

    @return void

*/
void Game::CaptureInput()
{
    sceneStateMachine.ProcessInput();
}
//Update mouse position relative to window (Vector2i)
//void Game::updateMousePositions()
//{
//    this->mousePosWindow = sf::Mouse::getPosition();
//
//}
//Broad collision detection
//bool Game::isColliding(sf::Sprite sprite1, sf::Sprite sprite2) {
//
//   return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
//}
//
//bool Game::isColliding(sf::RectangleShape simpleBoundingBox1, sf::RectangleShape simpleBoundingBox2) {
//
//    sf::FloatRect r1, r2;
//    r1.left = simpleBoundingBox1.getPosition().x;
//    r1.top = simpleBoundingBox1.getPosition().y;
//    r1.width = simpleBoundingBox1.getSize().x;
//    r1.height = simpleBoundingBox1.getSize().y;
//
//    r2.left = simpleBoundingBox2.getPosition().x;
//    r2.top = simpleBoundingBox2.getPosition().y;
//    r2.width = simpleBoundingBox2.getSize().x;
//    r2.height = simpleBoundingBox2.getSize().y;
//
//    if (r1.intersects(r2)) { 
//        return true;
//    }
//    return false;
//}

//void Game::checkCollisions()
//{
//    player.canMoveX = true;
//    player.canMoveY = true;
//    player.isColliding = false;
//    
//    for (auto& lyr : m_collisionBoxes) {
//        for (auto& rct : lyr.second) {
//            
//            if (isColliding(player.playerBoundingBox, rct))
//            {
//                player.isColliding = true;
//                if (player.xSpeed != 0 && player.ySpeed == 0)
//                {
//                    player.canMoveX = false;
//                }
//                else if (player.ySpeed != 0 && player.xSpeed == 0)
//                {
//                    player.canMoveY = false;
//                }
//                else {
//                    player.canMoveX = false;
//                    player.canMoveY = false;
//                }
//            }
//
//        }
//    }
// 
//}

//sf::Vector2f Game::getCenter(sf::RectangleShape object)
//{
//    sf::Vector2f center;
//    center.x = (object.getSize().x / 2.f) * object.getScale().x;
//    center.y = (object.getSize().y / 2.f) * object.getScale().y;
//
//
//    return center;
//}

/*!
 * Just a helper function to get the common path to the actual image files
 * @param tileset
 * @return
 */
//fs::path Game::getTilesetImagePath(const tson::Tileset& tileset)
//{
//    fs::path path = fs::path(tileset.getImagePath().generic_string()); //tileset.getImage().filename());
//    return path;
//}

/*
    Renders game objects.

    -Clear old frames
    -Render objects
    -Display frame in window

    @return void
    
*/
void Game::Draw()
{
    window.BeginDraw();
    sceneStateMachine.Draw(window);
    window.EndDraw();

    /*this->window->clear();
    this->window->setView(mainView);*/
    //Draw game objects
    
    //Below player
    /*sf::Vector2i centerTileIndex;
    centerTileIndex.x = (this->mainView.getCenter().x / 32);
    centerTileIndex.y = (this->mainView.getCenter().y / 32);
    sf::Vector2i tileBegin, tileEnd;
    if (centerTileIndex.x - renderDistance > 0 && centerTileIndex.y - renderDistance > 0) {
        tileBegin = sf::Vector2i(centerTileIndex.x - renderDistance, centerTileIndex.y - renderDistance);
    }
    else {
        tileBegin = sf::Vector2i(centerTileIndex.x, centerTileIndex.y);
    }
    if (centerTileIndex.x + renderDistance < this->mainView.getSize().x / 32 && centerTileIndex.y + renderDistance < this->mainView.getSize().x / 32) {
        tileEnd = sf::Vector2i(centerTileIndex.x + renderDistance, centerTileIndex.y + renderDistance);
    }
    else {
        tileEnd = sf::Vector2i(centerTileIndex.x, centerTileIndex.y);
    }

    for (auto& layer : tilesToDraw) {


        std::map<sf::Vector2i, my_Tile> renderedTiles;
        renderedTiles.insert((tilesToDraw[layer[tileBegin]], tilesToDraw[layer[tileEnd]]));

        for (auto& t : renderedTiles) {

          window.Draw(t.second);
        }
    }*/
    /*if (player.isVisible) {
        window.Draw(player);
    }*/

    /*for (auto& lyr : m_collisionBoxes) {
        for (auto& rct : lyr.second) {
            rct.setFillColor(sf::Color::Cyan);
            window.Draw(rct);
        }
            

    }*/
    /*if (!player.isVisible) {
        player.isVisible = true;
        player.playerBoundingBox.setFillColor(sf::Color::Red);
        this->window->draw(player.playerBoundingBox);
    }*/
   

     //Above player
    //for (auto& it : housesToDraw) {
    //    it.setTextureRect(it.roofTexture);
    //    it.setPosition(it.getPosition().x, it.getPosition().y - it.offsetY);
    //    this->window->draw(it);
    //   // this->window->draw(it.wallsBoundingBox); 
    //}
    //for (auto& it : portals) {
    //    sf::FloatRect ir = it.getPortalBounds();
    //    sf::RectangleShape r;
    //    r.setPosition(ir.left, ir.top);
    //    r.setSize(sf::Vector2f(ir.width, ir.height));
    //    it.isActive ? r.setFillColor(sf::Color::Green) : r.setFillColor(sf::Color::Red);
    //    this->window->draw(r);
    //}
    
}

void Game::CalculateDeltaTime()
{
    deltaTime = clock.restart().asSeconds();
}

