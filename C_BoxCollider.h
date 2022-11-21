#pragma once
#ifndef C_BoxCollider_h
#define C_BoxCollider_h

#include <math.h>

#include "Component.h"
#include "C_Collider.h"

class C_BoxCollider : public C_Collider
{
public:
	C_BoxCollider(Object* owner) : C_Collider(owner), offset(sf::Vector2f(0.f, 0.f))
	{

	};
	
	virtual ~C_BoxCollider() = default;
	
	Manifold Intersects(std::shared_ptr<C_Collider> other) override;
	void ResolveOverlap(const Manifold& m) override;

	void SetCollidable(const sf::FloatRect& rect);
	const sf::FloatRect& GetCollidable();

	sf::FloatRect GetPreviousFrameCollidable() const;

	void SetOffset(const sf::Vector2f& offset);
	void SetOffset(float x, float y);

	void SetSize(const sf::Vector2f& size);
	void SetSize(float width, float height);

private:
	void SetPosition();

	sf::FloatRect AABB;
	sf::Vector2f offset;
};

#endif /* C_BoxCollider_h */