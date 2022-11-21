#include "C_RemoveObjectOnCollisionEnter.h"
#include "Object.h"


void C_RemoveObjectOnCollisionEnter::OnCollisionEnter(std::shared_ptr<C_BoxCollider> other)
{
	// Remove the projectile when it collides with any other object    
	owner->QueueForRemoval();
	
	
}