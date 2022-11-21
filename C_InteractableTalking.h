#pragma once
#ifndef C_InteractableTalking_h
#define C_InteractableTalking_h

#include "Component.h"
#include "C_Interactable.h"
#include "Debug.h"
#include "C_UIWorldLabel.h"
#include "ObjectCollection.h"

class C_InteractableTalking : public Component, public C_Interactable
{
public:
    C_InteractableTalking(Object* owner) : Component(owner), textToSay("Bye!")
    {

    };

    virtual ~C_InteractableTalking() = default;
   
    void OnInteraction(Object* other) override;

private:
    std::string textToSay;
};

#endif /* C_InteractableTalking_h */