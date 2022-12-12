#pragma once

#ifndef C_Observer_h
#define C_Observer_h

#include "Event.h"
#include "Component.h"
#include "C_KeyboardMovement.h"
#include "C_Animation.h"
#include "C_InteractWithObjects.h"
#include "XML_Parser.h"

#include <iostream>


class C_Observer : public Component
{
public:

    C_Observer(Object* owner) : Component(owner), lastEventHandledType("E_None")
    {

    };
    virtual ~C_Observer() = default;

    void handle(const Event& e);

    Event::DescriptorType lastEventHandledType;

    Event::DescriptorType currentEventType;

};


#endif //C_Observer_h