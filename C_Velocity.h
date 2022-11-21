#pragma once
#ifndef C_Velocity_h
#define C_Velocity_h

#include <math.h>

#include "Component.h"
#include "C_Mass.h"

class C_Velocity : public Component
{
public:
	C_Velocity(Object* owner) : Component(owner), maxVelocity(100.f), frictionCoefficant(0.7), accelerationRate(0.075)
	{
		//Friction Coefficant calculated for rubber on dry concrete
	};
	virtual ~C_Velocity() = default;

	void Awake() override;
	void Update(float deltaTime) override;

	void AddVelocity();
	
	void SetDirection(const sf::Vector2f& direction);
	void SetDirection(float x, float y);
	const sf::Vector2f& GetDirection() const;

	void Set(const sf::Vector2f& vel);
	void Set(float x, float y);
	sf::Vector2f Get();
	float GetDirectionalVelocity();

	void SetFrictionCoefficant(float frictionCoefficant) { this->frictionCoefficant = frictionCoefficant; };
	float GetFrictionCoefficant() { return this->frictionCoefficant; };

	void SetAccelerationRate(float accelerationRate) { this->accelerationRate = accelerationRate; };
	float GetAccelerationRate() { return this->accelerationRate; };

	void SetMaxVelocity(float maxVelocity) { this->maxVelocity = maxVelocity; };
	float GetMaxVelocity() { return maxVelocity; };

private:
	void ClampVelocity();
	void CalculateAcceleration(float deltaTime);
	
	sf::Vector2f velocity;
	float maxVelocity;
	sf::Vector2f acceleration;
	sf::Vector2f heading;

	float accelerationRate;
	float frictionCoefficant;
	float friction;
	const float gravity = -9.8;
	
	std::shared_ptr <C_Mass> mass;
	
};

#endif /* C_Velocity_h */