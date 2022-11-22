#include "C_Velocity.h"
#include "Object.h"


void C_Velocity::Awake()
{
	mass = owner->GetComponent<C_Mass>();
}

void C_Velocity::Update(float deltaTime)
{
	CalculateAcceleration(deltaTime);
	AddVelocity();
	owner->transform->AddPosition(velocity * deltaTime);
	
}

void C_Velocity::AddVelocity()
{
	ClampVelocity();

	velocity.x += (((heading.x * maxVelocity) - velocity.x) * acceleration.x) * velocityMultiplier;
	velocity.y += (((heading.y * maxVelocity) - velocity.y) * acceleration.y) * velocityMultiplier;
	
}

void C_Velocity::SetDirection(const sf::Vector2f& vel)
{
	heading = vel;
}

void C_Velocity::SetDirection(float x, float y)
{
	heading.x = x;
	heading.y = y;
}

const sf::Vector2f& C_Velocity::GetDirection() const
{
	return heading;
}

void C_Velocity::Set(const sf::Vector2f& vel)
{
	velocity = vel;
}

void C_Velocity::Set(float x, float y)
{
	velocity.x = x;
	velocity.y = y;
}

sf::Vector2f C_Velocity::Get()
{
	return velocity;
}
float C_Velocity::GetDirectionalVelocity()
{
	return std::sqrtf(std::powf(velocity.x, 2) + std::powf(velocity.y, 2));
}

void C_Velocity::ClampVelocity()
{
	float directionalVelocity = GetDirectionalVelocity();
	float oldVelocityX = velocity.x;
	float oldVelocityY = velocity.y;


	if (fabs(directionalVelocity) > maxVelocity)
	{
		if (velocity.x > 0.f)
		{
			velocity.x = std::sqrtf(std::powf(maxVelocity, 2) - std::powf(oldVelocityY, 2));
		}
		else
		{
			velocity.x = -std::sqrtf(std::powf(maxVelocity, 2) - std::powf(oldVelocityY, 2));;
		}
		
		if (velocity.y > 0.f)
		{
			velocity.y = std::sqrtf(std::powf(maxVelocity, 2) - std::powf(oldVelocityX, 2));;
		}
		else
		{
			velocity.y = -std::sqrtf(std::powf(maxVelocity, 2) - std::powf(oldVelocityX, 2));;
		}
	}
	else if (fabs(directionalVelocity) <= 1 && fabs(directionalVelocity) > 0)
	{
		velocity.x = 0;
		velocity.y = 0;
	}
}

void C_Velocity::CalculateAcceleration(float deltaTime)
{
	acceleration.x = accelerationRate;
	acceleration.y = accelerationRate;

	friction = (1 - frictionCoefficant * mass->GetMass()) / gravity;
	
	if (heading.x == 0)
	{
		acceleration.x *= friction;
	}
	if (heading.y == 0)
	{
		acceleration.y *= friction;
	}


}

