#pragma once
#ifndef C_InteractWithObjects_h
#define C_InteractWithObjects_h

#include "Component.h"
#include "C_Direction.h"
#include "Raycast.h"
#include "C_Interactable.h"

class C_InteractWithObjects : public Component
{
public:
    C_InteractWithObjects(Object* owner) : Component(owner), interactionDistance(60.f) {};

    virtual ~C_InteractWithObjects() = default;
  
    void Awake() override;

    void Update(float deltaTime) override;

private:
    std::shared_ptr<C_Direction> direction;

    float interactionDistance;
};

#endif /* C_InteractWithObjects_h */