#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "WorkingDirectory.h"
#include "ResourceAllocator.h"
class SceneSplashScreen : public Scene
{
public:

	SceneSplashScreen(WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window, ResourceAllocator<sf::Font>& fontAllocator) :
		Scene(workingDir, textureAllocator, window, fontAllocator), currentSeconds(0.f),
		showForSeconds(3.f) {};

	void OnCreate() override;
	void OnDestroy() override;
	void OnActivate() override;
	void Update(float deltaTime) override;
	void Draw(Window& window) override;
private:
	sf::Texture splashTexture;
	sf::Sprite splashSprite;
	// We want to show this scene for a set amount of time
	float showForSeconds;
	// How long the scene has currently been visible.
	float currentSeconds;
};