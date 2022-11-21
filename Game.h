#pragma once
#ifndef Game_h
#define Game_h

#include <SFML/Graphics.hpp>

#include "WorkingDirectory.h"
#include "window.h"
#include "Input.h"
#include "SceneStateMachine.h"
#include "SceneSplashScreen.h"
#include "SceneGame.h"

	typedef std::map<std::string, std::string> TextureMap; //Create a partial class that is a type of map that only accepts 2 strings
	//typedef std::map<int, int, int> AreaMap;

	//Macro for checking if a XML value is valid or if it gives an error
//#ifndef XMLCheckResult
//#define XMLCheckResult(a_eResult) if (a_eResult != tinyxml2::XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
//#endif

//Game engine class controlling the update and rendering of the game

class Game
{
public:
	//Constructors / Deconstructors
	Game();
	virtual ~Game();

	//Accessors
	

	//Functions
	void CaptureInput();
	void Update();
	void LateUpdate();
	void Draw();
	void CalculateDeltaTime();
	bool IsRunning() const;
	

private:
	//Functions
	void initVariables();

	//Variables
	Window window;
	WorkingDirectory workingDir;

	sf::Clock clock;
	float deltaTime;

	SceneStateMachine sceneStateMachine;
	ResourceAllocator<sf::Texture> textureAllocator;
	ResourceAllocator<sf::Font> fontAllocator;

	//Input input;
	//sf::Event event;
	//sf::VideoMode videoMode; //Videomode just keeps track of two ints as a size
	//sf::Vector2f playerSpawn = sf::Vector2f(450, 450);
	//bool inMenu = false;
	//float groundFriction;
	
	//Timer
	
	//const int global_animation_framerate = 10;
	
	//Mouse position
	//sf::Vector2i mousePosWindow;
	
	//Xml stuff
	//TextureMap t_textures;
	//std::string t_name;
	//std::vector<std::string> xmlFiles;

	//json stuff
	//fs::path m_basePath{};
	//tson::Project m_project;

	//std::map<uint32_t, std::string> jsonMapFiles;
	//std::map<uint32_t, std::string> jsonCharacterFiles;
	//std::map <std::string, std::vector< sf::RectangleShape>> m_collisionBoxes; //Key: layer name, Value: list of bounding box objects from tiled
	//std::unique_ptr<tson::Map> m_map;
	//std::map<std::string, std::unique_ptr<tson::Map>> m_projectMaps; //Non-world maps in project
	//std::vector<std::unique_ptr<tson::Map>> m_worldMaps; //World-related maps in project
	//std::vector<tson::WorldMapData> m_worldData; //World-related data in project
	//std::vector<bool> m_worldVisibilityFlags; //World-related data in project
	//std::vector<std::string> m_worldMapInfo;
	//std::vector<std::string> m_projectMapInfo;
	//tson::Map* m_currentMap;
	//std::string m_currentInfo;

	//int m_mapIndex{ 0 };
	//const int m_maxMapIndex{ 1000 };
	//tson::Vector2i m_positionOffset{ 0,0 }; //Used for world related stuff

	//std::map<std::string, std::unique_ptr<sf::Texture>> m_textures;
	//std::map<std::string, std::unique_ptr<Tile>> m_sprites;
	//std::map<uint32_t, tson::Animation*> m_animationUpdateQueue;

	//std::vector<std::map<sf::Vector2i, my_Tile>> tilesToDraw;
	
	//Game objects
	
	//Player player;
	//sf::Texture pTexture;
	//std::string pTextureName;
	//sf::Vector2f playerPreviousLocation;
	
	//sf::View mainView;
	//int renderDistance = 10;
	//std::vector<Enemy> enemies;

	//int currentLevel = GLC::UNASSIGNED_LOCATION;
	

	//Private functions
	
	//void initWindow();
	//void initTextures();
	//void initEnemies();
	//int initPlayer();
	//void loadLevel(uint32_t levelName);
	//void unloadLevel();
	//std::unique_ptr<tson::Map> parseMap(const std::string& filename = "Town_1_data.json", std::unique_ptr<tson::IDecompressor<std::vector<uint8_t>, std::vector<uint8_t>>> decompressor = nullptr);
	
	//void drawMap();
	//void updateAnimations(Tile& t);
	//void drawLayer(tson::Layer& layer);
	//void drawTileLayer(tson::Layer& layer);//, tson::Tileset* tileset);
	//void drawImageLayer(tson::Layer& layer);
	//void drawObjectLayer(tson::Layer& layer);
	//Tile* storeAndLoadImage(const std::string& image, const sf::Vector2f& position);
	//fs::path getTilesetImagePath(const tson::Tileset& tileset);
	//sf::Vector2f getTileOffset(int tileId, tson::Map* map, tson::Tileset* tileset);
	//bool parseProject(const std::string& filename);
	//void spawnEnemy();
	//void pollEvents();
	//void updateMousePositions();
	//bool isColliding(sf::Sprite sprite1, sf::Sprite sprite2);
	//bool isColliding(sf::RectangleShape simpleBoundingBox1, sf::RectangleShape simpleBoundingBox2);
	//void checkCollisions();
	//sf::Vector2f getCenter(sf::RectangleShape object);


	
	
	
};

#endif /* Game_h */