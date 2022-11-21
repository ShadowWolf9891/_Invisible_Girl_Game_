#pragma once
#ifndef C_SteeringBehaviourWallAvoidance_h
#define C_SteeringBehaviourWallAvoidance_h

#include "C_SteeringBehaviour.h"
#include "Raycast.h"
#include "C_Velocity.h"

class C_SteeringBehaviourWallAvoidance : public C_SteeringBehaviour
{
public:
    C_SteeringBehaviourWallAvoidance(Object* owner) : C_SteeringBehaviour(owner), lookDistance(100.f)
    {

    };
    virtual ~C_SteeringBehaviourWallAvoidance() = default;

    void Awake() override;

    const sf::Vector2f GetForce() override;

private:
    std::shared_ptr<C_Velocity> velocity;
    float lookDistance;
};


#endif /* C_SteeringBehaviourWallAvoidance_h */