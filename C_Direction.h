#pragma once
#ifndef C_Direction_h
#define C_Direction_h
#include "Component.h"
#include "Animation.h"
#include "C_Velocity.h"

class C_Direction : public Component
{
public:
	C_Direction(Object* owner) : Component(owner), currentDir(FacingDirection::Down)
	{

	};
	virtual ~C_Direction() = default;
	
	void Awake() override;

	FacingDirection Get();

	sf::Vector2i GetHeading();

private:
	std::shared_ptr<C_Velocity> velocity;
	FacingDirection currentDir;
};
#endif /* C_Direction_h */