#pragma once
#ifndef C_Collidable_h
#define C_Collidable_h
#include <memory>
#include "C_BoxCollider.h"

class Object;

class C_Collidable
{
public:
	virtual void OnCollisionEnter(std::shared_ptr<C_BoxCollider> other) {};
	virtual void OnCollisionStay(std::shared_ptr<C_BoxCollider> other) {};
	virtual void OnCollisionExit(std::shared_ptr<C_BoxCollider> other) {};
};
#endif /* C_Collidable_h */