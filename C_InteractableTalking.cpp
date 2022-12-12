#include "C_InteractableTalking.h"

void C_InteractableTalking::OnInteraction(Object* other)
{
    std::shared_ptr<Object> dialogueObj = std::make_shared<Object>(owner->context);

    auto dialogue = owner->context->xml_parser->LoadDialogueDataFromFile(dialogueFilename);
    auto dComponent = dialogueObj->AddComponent<C_Dialogue>();
    dComponent->SetDrawLayer(DrawLayer::UI);
    dComponent->allDialogue = dialogue;

    owner->context->objects->Add(dialogueObj);


}