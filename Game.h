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
#include "SceneMainMenu.h"
#include "imgui.h"
#include "imgui-SFML.h"

typedef std::map<std::string, std::string> TextureMap; //Create a partial class that is a type of map that only accepts 2 strings
	
//Game engine class controlling the update and rendering of the game

class Game
{
public:
	//Constructors / Deconstructors
	Game();
	virtual ~Game();

	

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
	void LoadFonts();
	void LoadScreens();

	//Variables
	Window window;
	WorkingDirectory workingDir;

	sf::Clock clock;
	float deltaTime;

	SceneStateMachine sceneStateMachine;
	ResourceAllocator<sf::Texture> textureAllocator;
	ResourceAllocator<sf::Font> fontAllocator;


	bool firstLoad;

};

#endif /* Game_h */