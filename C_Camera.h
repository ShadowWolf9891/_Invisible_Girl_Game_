#pragma once

#ifndef C_Camera_h
#define C_Camera_h
#include "Component.h"

class C_Camera : public Component
{
public:
	C_Camera(Object* owner) : Component(owner)
	{

	};

	virtual ~C_Camera() = default;
	
	void LateUpdate(float deltaTime) override;
};
#endif /* C_Camera_h */