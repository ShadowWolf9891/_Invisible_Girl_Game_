#include "C_Observer.h"
#include "Object.h"

void C_Observer::handle(const Event& e)
{
    currentEventType = e.getType();

    for (auto it = strToEvent.begin(); it != strToEvent.end(); it++) {
        if (it->second == currentEventType)
        {
            auto it2 = strToAnimState.find(it->first);
            if (it2 != strToAnimState.end()) {
                std::shared_ptr CAnim = owner->GetComponent<C_Animation>();
                CAnim->SetAnimationState(it2->second);
            }
           
            break;
        }
    }

    float targetSpeed = 1.f;

    if (currentEventType == E_Walk::descriptor) targetSpeed = 0.5f;
    else if (currentEventType == E_Run::descriptor) targetSpeed = 1.f;
    else if (currentEventType == E_BowDraw::descriptor) targetSpeed = 0.1f;
    else if (currentEventType == E_BowSheath::descriptor) targetSpeed = 0.1f;
    else if (currentEventType == E_BowMove::descriptor) targetSpeed = 0.25f;

    owner->GetComponent<C_Velocity>()->SetVelocityMultiplier(targetSpeed);

    lastEventHandledType = currentEventType;

    // This demonstrates how to obtain the underlying event type in case a slot is set up to handle multiple events of different types.
        const Event& testEvent = static_cast<const Event&>(e);
        std::cout << testEvent.getName() << " was triggered." << std::endl;
}