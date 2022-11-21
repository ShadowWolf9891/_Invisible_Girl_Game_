#include "C_collider.h"


CollisionLayer C_Collider::GetLayer() const
{
	return layer;
}
void C_Collider::SetLayer(CollisionLayer layer)
{
	this->layer = layer;
}