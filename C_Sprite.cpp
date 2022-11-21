#include "C_Sprite.h"
#include "Object.h" //Used to get owner, otherwise we know object exists, but cant access it




void C_Sprite::Load(const std::string& filePath)
{
	int textureID = owner->context->textureAllocator->Add(filePath);
	if (textureID >= 0 && textureID != currentTextureID)
	{
		currentTextureID = textureID;
		std::shared_ptr<sf::Texture> texture = owner->context->textureAllocator->Get(textureID);
		sprite.setTexture(*texture);
	}

}
void C_Sprite::Load(int id)
{
	if (id >= 0 && id != currentTextureID)
	{
		currentTextureID = id;
		std::shared_ptr<sf::Texture> texture = owner->context->textureAllocator->Get(id);
		sprite.setTexture(*texture);
	}
}
void C_Sprite::LateUpdate(float deltaTime)
{
	sf::Vector2f pos = owner->transform->GetPosition();
	const sf::IntRect& spriteBounds = sprite.getTextureRect();
	const sf::Vector2f& spriteScale = sprite.getScale();
	sprite.setPosition(sf::Vector2(
		pos.x - ((abs(spriteBounds.width) * 0.5f) * spriteScale.x),
		pos.y - ((abs(spriteBounds.height) * 0.5f) * spriteScale.y)));
}

void C_Sprite::Draw(Window& window)
{
	window.Draw(sprite);
}

void C_Sprite::SetTextureRect(int x, int y, int width, int height)
{
	sprite.setTextureRect(sf::IntRect(sf::Vector2(x, y), sf::Vector2(width, height)));
}

void C_Sprite::SetTextureRect(const sf::IntRect& rect)
{
	sprite.setTextureRect(rect);
}

bool C_Sprite::ContinueToDraw() const
{
	return !owner->IsQueuedForRemoval();
}

void C_Sprite::SetScale(float x, float y)
{
	sprite.setScale(sf::Vector2(x, y));
}