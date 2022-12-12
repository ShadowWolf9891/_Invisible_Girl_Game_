#pragma once
#ifndef C_InteractableTalking_h
#define C_InteractableTalking_h

#include "Component.h"
#include "C_Interactable.h"
#include "Debug.h"
#include "C_Dialogue.h"
#include "ObjectCollection.h"
#include "XML_Parser.h"

class C_InteractableTalking : public Component, public C_Interactable
{
public:
    C_InteractableTalking(Object* owner) : Component(owner)
    {

    };

    virtual ~C_InteractableTalking() = default;
   
    void OnInteraction(Object* other) override;

    void SetFilename(std::string filename) { dialogueFilename = filename; };

private:
    std::string dialogueFilename;
};

#endif /* C_InteractableTalking_h */