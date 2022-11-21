#include "C_BoxCollider.h"
#include "Object.h"



Manifold C_BoxCollider::Intersects(std::shared_ptr<C_Collider> other)
{
	Manifold m;
	m.colliding = false; // We default to not colliding.

	std::shared_ptr<C_BoxCollider> boxCollider = std::dynamic_pointer_cast<C_BoxCollider>(other); //Try to cast other collider to box collider, fix after other types
	if (boxCollider) //If cast failed return nullptr
	{
		const sf::FloatRect& rect1 = GetCollidable();
		const sf::FloatRect& rect2 = boxCollider->GetCollidable();

		if (rect1.intersects(rect2)) // Use SFML to check collision
		{
			m.colliding = true; //Build manifest file and return it
			m.other = &rect2;
		}
	}

	return m;
}

void C_BoxCollider::ResolveOverlap(const Manifold& m)
{
	auto transform = owner->transform;

	if (transform->isStatic()) { return; } // If it is static we dont want to move it
	
	const sf::FloatRect& rect1 = GetCollidable();
	const sf::FloatRect* rect2 = m.other;
	const float R1_Top = rect1.top, R1_Bot = rect1.top + rect1.height, R1_Left = rect1.left, R1_Right = rect1.left + rect1.width;
	const float R2_Top = rect2->top, R2_Bot = rect2->top + rect2->height, R2_Left = rect2->left, R2_Right = rect2->left + rect2->width;

	float resolveX = 0, resolveY = 0; //Stores distance we have to move the object to resolve collision

	if (R1_Bot > R2_Top && R1_Top < R2_Bot) //If R1 is between the top and bottom of r2, must be colliding from side
	{
		if (R1_Left < R2_Right && R1_Right > R2_Right) //Colliding on rect1's left, rect2's right
		{
			resolveX = R2_Right - R1_Left;
		}
		else if (R1_Right > R2_Left && R1_Left < R2_Left) 
		{
			resolveX = -(R1_Right - R2_Left);
		}
		
	}
	if (R1_Right > R2_Left && R1_Left < R2_Right) {

		if (R1_Bot > R2_Top && R1_Top < R2_Top) 
		{
			resolveY = -(R1_Bot - R2_Top);
		}
		else if(R1_Top < R2_Bot && R1_Bot > R2_Bot)
		{
			resolveY = R2_Bot - R1_Top;
		}
		
	}
	if (resolveX == 0) {
		if (resolveY != 0) {

			transform->AddPosition(0, resolveY);
		}
	}
	else {
		if (resolveY == 0 || fabs(resolveX) < fabs(resolveY)) {

			transform->AddPosition(resolveX, 0);
		}
		else {
			transform->AddPosition(0, resolveY);
		}
	}
		
	
	
}
//TODO: As we're updaing the rects position manually we only need to apss the width and height of the rect and an offset.
void C_BoxCollider::SetCollidable(const sf::FloatRect& rect)
{
	AABB = rect;
	SetPosition();
}
const sf::FloatRect& C_BoxCollider::GetCollidable()
{
	SetPosition();
	return AABB;
}
sf::FloatRect C_BoxCollider::GetPreviousFrameCollidable() const
{
	const sf::Vector2f& pos = owner->transform->GetPreviousFramePosition();

	sf::FloatRect prevAABB;
	prevAABB.left = pos.x - (AABB.width / 2) + offset.x;
	prevAABB.top = pos.y - (AABB.height / 2) + offset.y;

	return prevAABB;
}

void C_BoxCollider::SetOffset(const sf::Vector2f& offset)
{
	this->offset = offset;
}
void C_BoxCollider::SetOffset(float x, float y)
{
	offset.x = x;
	offset.y = y;
}
void C_BoxCollider::SetSize(const sf::Vector2f& size)
{
	AABB.width = size.x;
	AABB.height = size.y;
}
void C_BoxCollider::SetSize(float width, float height)
{
	AABB.width = width;
	AABB.height = height;
}
void C_BoxCollider::SetPosition()
{
	const sf::Vector2f& pos = owner->transform->GetPosition();
	AABB.left = pos.x - (AABB.width / 2) + offset.x;
	AABB.top = pos.y - (AABB.height / 2) + offset.y;
}