#pragma once
#ifndef C_Collider_h
#define C_Collider_h

#include<memory>
#include <SFML/Graphics.hpp>
#include "Component.h"
// enum ints used when defining collision layers
enum class CollisionLayer
{
	Default = 1,    // bit 0
	Player = 2,     // bit 1
	Tile = 3,       // bit 2
	Projectile = 4, //bit 3
	NPC = 5			//bit 4

};
struct Manifold
{
	bool colliding = false;
	const sf::FloatRect* other;
};

class C_Collider : public Component
{
public:
	C_Collider(Object* owner) : Component(owner), layer(CollisionLayer::Default)
	{

	};

	virtual Manifold Intersects(std::shared_ptr<C_Collider> other) = 0;
	virtual void ResolveOverlap(const Manifold& m) = 0;

	CollisionLayer GetLayer() const;
	void SetLayer(CollisionLayer layer);
private:
	CollisionLayer layer;
};

#endif /* C_Collider_h */