#pragma once
#ifndef C_SteeringBehaviourChase_h
#define C_SteeringBehaviourChase_h

#include "C_SteeringBehaviour.h"

class C_SteeringBehaviourChase : public C_SteeringBehaviour
{
public:
    C_SteeringBehaviourChase(Object* owner) : C_SteeringBehaviour(owner), targetTag(Tag::Default), sightRadius(500.f), chaseSpeed(0.75f)
    {

    };

    virtual ~C_SteeringBehaviourChase() = default;
    const sf::Vector2f GetForce() override;

    void SetTarget(Tag tag);
    void SetSightRadius(float radius);
    void SetChaseSpeed(float speed);

private:
    float sightRadius;
    float chaseSpeed;
    Tag targetTag;
};

#endif /* C_SteeringBehaviourChase_h */