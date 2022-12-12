#include "C_InteractWithObjects.h"
#include "Object.h"


void C_InteractWithObjects::Awake()
{
    direction = owner->GetComponent<C_Direction>();
}


/*Check interaction between the ownerand other nearby interactable objects, then trigger their OnInteraction code.
Return if a collision has happened or not.*/
bool C_InteractWithObjects::CheckInteraction()
{

    sf::Vector2i heading = direction->GetHeading();

    const sf::Vector2f& startPoint = owner->transform->GetPosition();

    sf::Vector2f endPoint;

    endPoint.x = startPoint.x + (heading.x * interactionDistance);
    endPoint.y = startPoint.y + (heading.y * interactionDistance);

    RaycastResult result = owner->context->raycast->Cast(startPoint, endPoint, owner->instanceID->Get());

    if (result.collision != nullptr)
    {
        // Retrieve all interactable components
        auto interactables = result.collision->GetComponents<C_Interactable>();

        for (auto& interactable : interactables)
        {
            interactable->OnInteraction(owner);
        }
    }
    else {

        return false;
    }
    return true;
}