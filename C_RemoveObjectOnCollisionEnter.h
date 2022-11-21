#pragma once
#ifndef C_RemoveObjectOnCollisionEnter_h
#define C_RemoveObjectOnCollisionEnter_h
#include "Component.h"
#include "C_Collidable.h"
class C_RemoveObjectOnCollisionEnter : public Component, public C_Collidable
{
public:
	C_RemoveObjectOnCollisionEnter(Object* owner) : Component(owner)
	{

	};
	virtual ~C_RemoveObjectOnCollisionEnter() = default;

	void OnCollisionEnter(std::shared_ptr<C_BoxCollider> other) override;
};
#endif /* C_RemoveObjectOnCollisionEnter_h */