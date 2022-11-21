#pragma once
#ifndef C_WalkInLine_h
#define C_WalkInLine_h

#include "Component.h"
#include "C_Interactable.h"
#include "C_Velocity.h"

class C_WalkInLine : public Component, public C_Interactable
{
public:
    C_WalkInLine(Object* owner) : Component(owner), moveSpeed(0.75f) {};

    virtual ~C_WalkInLine() = default;
    void Awake() override;

    void OnInteraction(Object* other) override;

private:
    std::shared_ptr<C_Velocity> velocity;
    float moveSpeed;
};

#endif /* C_WalkInLine_h */