#pragma once
#ifndef C_Interactable_h
#define C_Interactable_h

#include "Object.h"

class C_Interactable
{
public:
    virtual void OnInteraction(Object* other) = 0;
};

#endif /* C_Interactable_h */