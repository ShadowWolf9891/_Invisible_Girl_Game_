#pragma once
#ifndef C_FollowMouse_h
#define C_FollowMouse_h

#include "Component.h"
#include "C_Transform.h"
#include "Object.h"
#include "Input.h"

class C_FollowMouse : public Component
{
public:
	C_FollowMouse(Object* owner) : Component(owner)
	{

	};

	virtual ~C_FollowMouse() = default;

	void Awake() override
	{
		transformComponent = owner->GetComponent<C_Transform>();
	};

	void LateUpdate(float deltaTime) override
	{
		transformComponent->SetPosition(owner->context->window->GetRenderWindow().mapPixelToCoords(sf::Mouse::getPosition()));
	};

private:

	std::shared_ptr<C_Transform> transformComponent;
};

#endif /* C_FollowMouse_h */