#pragma once
#ifndef C_KeyboardMovement_h
#define C_KeyboardMovement_h

#include "Component.h"
#include "Input.h"
#include "C_Velocity.h"
#include "C_Animation.h"

class C_KeyboardMovement : public Component
{
public:
	C_KeyboardMovement(Object* owner) : Component(owner), moveSpeed(1)
	{

	};

	virtual ~C_KeyboardMovement() = default;
	
	void Awake() override;

	void SetMovementSpeed(float moveSpeed);

	void Update(float deltaTime) override;

private:

	float moveSpeed;
	
	std::shared_ptr<C_Velocity> velocity;
	std::shared_ptr<C_Animation> animation;
};

#endif /* C_KeyboardMovement_h */