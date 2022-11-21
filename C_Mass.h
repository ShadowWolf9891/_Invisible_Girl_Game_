#pragma once
#ifndef C_Mass_h
#define C_Mass_h

#include "Component.h"
class C_Mass : public Component
{
public:
	C_Mass(Object* owner) : Component(owner), mass(100)
	{

	};
	virtual ~C_Mass() = default;
	
	void SetMass(float m);
	float GetMass();

private:
	float mass;

};
#endif /* C_Mass_h */