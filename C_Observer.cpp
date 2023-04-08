#include "C_Observer.h"
#include "Object.h"




void C_Observer::Awake()
{
    cAnim = owner->GetComponent<C_Animation>();
    cVel = owner->GetComponent<C_Velocity>();
}

void C_Observer::handle(const Event& e)
{
    if (e.GetTargetID() != owner->instanceID->Get()) {
        return;
    } 
    currentEventType = e.getType();

    if (currentEventType == lastEventHandledType) return;
    
    std::string eventType = currentEventType;

    if (eventType == "E_None") e_None();
    else if (eventType == "E_Idle") e_Idle();
    else if (eventType == "E_Walk") e_Walk();
    else if (eventType == "E_Run") e_Run();
    else if (eventType == "E_DrawWeapon") e_DrawWeapon();
    else if (eventType == "E_Parry")e_Parry();
    else if (eventType == "E_Dodge")e_Dodge();
    else if (eventType == "E_Hurt")e_Hurt();
    else if (eventType == "E_Dead")e_Dead();
    else if (eventType == "E_Crouch")e_Crouch();
    else if (eventType == "E_Lunge")e_Lunge();
    else if (eventType == "E_Attack")e_Attack();
    else if (eventType == "E_Interact")e_Interact();

        //Other events calls go here, definition in .h

    else
    {
        std::cout << eventType << " could not be found, and has not been handled." << std::endl;
        return;
    }
       
    

    lastEventHandledType = currentEventType;


    // This demonstrates how to obtain the underlying event type in case a slot is set up to handle multiple events of different types.
        const Event& testEvent = static_cast<const Event&>(e);
        std::cout << testEvent.getName() << " was triggered by " << owner->GetName() << std::endl;
}