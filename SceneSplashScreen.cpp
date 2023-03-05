#include "SceneSplashScreen.h"

void SceneSplashScreen::OnCreate()
{
	Scene::OnCreate();

	// We�fll initialise our splash screen image here.
	int textureID = textureAllocator.Add(workingDir.Get() + "Assets/RPGW_Harbor_v1.0/Ships.png");
	if (textureID >= 0)
	{
		std::shared_ptr<sf::Texture> texture = textureAllocator.Get(textureID);
		splashSprite.setTexture(*texture);
		sf::FloatRect spriteSize = splashSprite.getLocalBounds();
		splashSprite.setOrigin(sf::Vector2f(spriteSize.width * 0.5f, spriteSize.height * 0.5f));
		splashSprite.setScale(sf::Vector2f(0.5f, 0.5f));
		sf::Vector2u windowCenter = window.GetCenter();
		splashSprite.setPosition(sf::Vector2f(windowCenter.x, windowCenter.y));
	}
}
void SceneSplashScreen::OnActivate()
{
	// Resets the currentSeconds count whenever the scene is activated.
	currentSeconds = 0.f;
}
void SceneSplashScreen::OnDestroy() {


}
void SceneSplashScreen::Update(float deltaTime)
{
	Scene::Update(deltaTime);
	currentSeconds += deltaTime;
	if (currentSeconds >= showForSeconds)
	{
		// Switches states.
		//sceneStateMachine.SwitchTo(switchToState);
		switchScene = true;

	}
}
void SceneSplashScreen::Draw(Window& window)
{
	Scene::Draw(window);
	window.Draw(splashSprite);
}