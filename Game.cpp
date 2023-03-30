#include "Game.h"

//Constructors / Deconstructors

Game::Game() : window("Invisible Girl Game"), firstLoad(true)
{
	this->initVariables();
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
    // Application init: create a dear imgui context, setup some options, load fonts
    /*ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();*/
    // TODO: Set optional io.ConfigFlags values, e.g. 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard' to enable keyboard controls.
    // TODO: Fill optional fields of the io structure later.

    
    window.GetRenderWindow().setFramerateLimit(60);

    ImGui::SFML::Init(window.GetRenderWindow()); // Initialize sfml
   

    LoadFonts();
    LoadScreens();

    
    //Restart clock 
    deltaTime = clock.restart().asSeconds();
    
}

void Game::LoadFonts()
{
    //Add fonts for Sfml here...
    fontAllocator.Add(workingDir.Get() + "data/fonts/Deutsch.ttf"); //0
    fontAllocator.Add(workingDir.Get() + "data/fonts/Lato-Regular.ttf"); //1
    fontAllocator.Add(workingDir.Get() + "data/fonts/Lato-Italic.ttf"); //2
    fontAllocator.Add(workingDir.Get() + "data/fonts/Lato-Bold.ttf"); //3
    fontAllocator.Add(workingDir.Get() + "data/fonts/Lato-Black.ttf"); //4

    //Add fonts for Imgui here...
    //Default font is 0
    ImGui::GetIO().Fonts->AddFontFromFileTTF("../data/fonts/Deutsch.ttf", 72.0f); //1           Big buttons
    ImGui::GetIO().Fonts->AddFontFromFileTTF("../data/fonts/Lato-Regular.ttf", 24.0f); //2      Dialogue
    ImGui::GetIO().Fonts->AddFontFromFileTTF("../data/fonts/Lato-Italic.ttf", 24.0f); //3       Special Dialogue
    ImGui::GetIO().Fonts->AddFontFromFileTTF("../data/fonts/Lato-Bold.ttf", 24.0f); //4         Important Dialogue
    ImGui::GetIO().Fonts->AddFontFromFileTTF("../data/fonts/Lato-Black.ttf", 36.0f); //5        Names and titles


    //Must be called to enable the correct font texture. Only once after all fonts loaded.
    ImGui::SFML::UpdateFontTexture(); 
}

void Game::LoadScreens()
{
    //Create game screens
    std::shared_ptr<SceneSplashScreen> splashScreen = std::make_shared<SceneSplashScreen>(workingDir, textureAllocator, window, fontAllocator); //Smart pointer to splash screen scene
    std::shared_ptr<SceneMainMenu> mainMenuScreen = std::make_shared<SceneMainMenu>(workingDir, textureAllocator, window, fontAllocator);
    std::shared_ptr<SceneGame> gameScene = std::make_shared<SceneGame>(workingDir, textureAllocator, window, fontAllocator); //Smart pointer to game scene

    //Add scenes to scenestatemachine and return the ID's
    unsigned int splashScreenID = sceneStateMachine.Add(splashScreen); 
    unsigned int gameSceneID = sceneStateMachine.Add(gameScene);
    unsigned int mainMenuID = sceneStateMachine.Add(mainMenuScreen);

    //Set what scene to change to when SwitchTo is called

    splashScreen->SetSwitchToScene(mainMenuID); //Set splashscreen to transition to gamescreen when switched to
    mainMenuScreen->SetSwitchToScene(gameSceneID);
    gameScene->SetSwitchToScene(mainMenuID);
    sceneStateMachine.SwitchTo(splashScreenID); //Set game to start on splashscreen
}

/*
    Updates the parts that change of the game.

    -Check for events
    -Render screen

    @return void

*/
void Game::Update()
{
    window.Update(clock.getElapsedTime());
    ImGui::SFML::Update(window.GetRenderWindow(), clock.getElapsedTime());
    sceneStateMachine.Update(deltaTime);
    ImGui::EndFrame;
}

void Game::LateUpdate()
{
    sceneStateMachine.LateUpdate(deltaTime);
}

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

/*
    Renders game objects.

    -Clear old frames
    -Render objects
    -Display frame in window

    @return void
    
*/
void Game::Draw()
{
    if (firstLoad)
    {
        window.GetRenderWindow().resetGLStates();
        firstLoad = false;
    }
    window.BeginDraw();
    sceneStateMachine.Draw(window);
    ImGui::SFML::Render(window.GetRenderWindow());
    window.EndDraw();

}

void Game::CalculateDeltaTime()
{
    deltaTime = clock.restart().asSeconds();
}

