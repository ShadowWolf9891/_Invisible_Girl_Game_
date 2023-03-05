#pragma once
#ifndef C_Sprite_h
#define C_Sprite_h

#include <math.h>

#include "Component.h"
#include "C_Transform.h"
#include "ResourceAllocator.h"
#include "C_Drawable.h"
class C_Sprite : public Component, public C_Drawable
{
public:
	C_Sprite(Object* owner) : Component(owner), currentTextureID(-1)
	{

	};
	virtual ~C_Sprite() = default;

	void Load(const std::string& filePath); // Loads a sprite from file.
	void Load(int id); //Load by texture ID

	void Draw(Window& window) override;// We override the draw method so we can draw our sprite.
	void LateUpdate(float deltaTime) override; //Update our sprite based on our position.
	bool ContinueToDraw() const override;

	void SetTextureRect(int x, int y, int width, int height);
	void SetTextureRect(const sf::IntRect& rect);
	
	void SetScale(float x, float y);
	sf::Vector2f GetScale() { return sprite.getScale(); };

	//Local bounds of sprite, ignoring scale
	sf::FloatRect GetLocalBounds()
	{
		return sprite.getLocalBounds();
	};

	sf::FloatRect GetGlobalBounds()
	{
		return sprite.getGlobalBounds();
	};

private:
	sf::Sprite sprite;
	int currentTextureID;
};

#endif /* C_Sprite_h */
