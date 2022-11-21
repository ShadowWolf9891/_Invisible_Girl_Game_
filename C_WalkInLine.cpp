#include "C_WalkInLine.h"
#include "Object.h"

void C_WalkInLine::Awake()
{
    velocity = owner->GetComponent<C_Velocity>();
}

void C_WalkInLine::OnInteraction(Object* other)
{
    velocity->Set(moveSpeed, 0.f);
}
