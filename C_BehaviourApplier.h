#pragma once
#ifndef C_BehaviourApplier_h
#define C_BehaviourApplier_h

#include "Component.h"
#include "C_SteeringBehaviour.h"
#include "C_Velocity.h"

class C_BehaviourApplier : public Component
{
public:
    C_BehaviourApplier(Object* owner) : Component(owner)
    {
    };

    virtual ~C_BehaviourApplier() = default;
   
    void Awake() override;
    void Update(float deltaTime) override;

private:
    inline float SqrMagnitude(const sf::Vector2f& v);

    std::vector<std::shared_ptr<C_SteeringBehaviour>> behaviours;
    std::shared_ptr<C_Velocity> velocity;
};


#endif /* C_BehaviourApplier_h */