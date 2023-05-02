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

    C_Observer(Object* owner) : Component(owner), lastEventHandledType("E_None"), currentEventType("E_None"), curWeapon(WeaponType::NOWEAPON),
        equippedWeapon(WeaponType::NOWEAPON)
    {

    };
    virtual ~C_Observer() = default;

    void Awake() override;

    void handle(const Event& e);

    Event::DescriptorType lastEventHandledType;
    Event::DescriptorType currentEventType;

private:

    //Handlers for specific events

    void e_None()
    {
        cVel->SetVelocityMultiplier(idleSpeed);
        cAnim->SetAnimationState(AnimationState::None);
    };
    void e_Idle()
    {
        cVel->SetVelocityMultiplier(idleSpeed);

        switch (curWeapon)
        {
        case NOWEAPON:
            cAnim->SetAnimationState(AnimationState::Idle);
            break;
        case SWORD:
            break;
        case BOW:
            cAnim->SetAnimationState(AnimationState::BowIdle);
            break;
        case SPEAR:
            break;
        default:
            std::cout << "Unknown Weapon Type." << std::endl;
            break;
        }
    };
    void e_Walk()
    {
        switch (curWeapon)
        {
        case NOWEAPON:
            cVel->SetVelocityMultiplier(walkSpeed);
            cAnim->SetAnimationState(AnimationState::Walk);
            break;
        case SWORD:
            break;
        case BOW:
            cVel->SetVelocityMultiplier(bowOutSpeed);
            cAnim->SetAnimationState(AnimationState::BowMove);
            break;
        case SPEAR:
            break;
        default:
            std::cout << "Unknown Weapon Type." << std::endl;
            break;
        }
    };
    void e_Run()
    {
        switch (curWeapon)
        {
        case NOWEAPON:
            cVel->SetVelocityMultiplier(runSpeed);
            cAnim->SetAnimationState(AnimationState::Run);
            break;
        default:
            e_Dodge();
            break;
        }
    };
    void e_DrawWeapon()
    {
        switch (equippedWeapon)
        {
        case NOWEAPON:
            std::cout << "No weapon equipped to " << owner->tag << std::endl;
            break;
        case SWORD:
            if (curWeapon == NOWEAPON)
            {
                curWeapon = WeaponType::SWORD;
                //cAnim->SetAnimationState(AnimationState::SwordDraw);
            }
            else
            {
                curWeapon = WeaponType::NOWEAPON;
                //cAnim->SetAnimationState(AnimationState::SwordSheath);
            }
            break;

        case BOW:
            if (curWeapon == NOWEAPON)
            {
                curWeapon = WeaponType::BOW;
                cAnim->SetAnimationState(AnimationState::BowDraw);
            }
            else
            {
                curWeapon = WeaponType::NOWEAPON;
                cAnim->SetAnimationState(AnimationState::BowSheath);
            }
            break;
        case SPEAR:
            if (curWeapon == NOWEAPON)
            {
                curWeapon = WeaponType::SPEAR;
                //cAnim->SetAnimationState(AnimationState::SpearDraw);
            }
            else
            {
                curWeapon = WeaponType::NOWEAPON;
                //cAnim->SetAnimationState(AnimationState::SpearSheath);
            }
            break;
        default:
            std::cout << "Unknown Weapon Type." << std::endl;
            break;
        }
    };
    void e_Parry()
    {
        //May need to check if parry is finished and reset speed
        switch (curWeapon)
        {
        case NOWEAPON:
            e_DrawWeapon();
            break;
        case SWORD:
            cVel->SetVelocityMultiplier(drawSpeed);
            //cAnim->SetAnimationState(AnimationState::SwordParry);
            break;
        case BOW:
            cVel->SetVelocityMultiplier(drawSpeed);
            cAnim->SetAnimationState(AnimationState::BowParry);
            break;
        case SPEAR:
            cVel->SetVelocityMultiplier(drawSpeed);
            //cAnim->SetAnimationState(AnimationState::SpearParry);
            break;
        default:
            std::cout << "Unknown Weapon Type." << std::endl;
            break;
        }
    };
    void e_Dodge()
    {
        switch (curWeapon)
        {
        case NOWEAPON:
            e_Run();
            break;
        case SWORD:

            //cAnim->SetAnimationState(AnimationState::SwordDodge);
            break;
        case BOW:

            cAnim->SetAnimationState(AnimationState::BowDodge);
            break;
        case SPEAR:

            //cAnim->SetAnimationState(AnimationState::SpearDodge);
            break;
        default:
            std::cout << "Unknown Weapon Type." << std::endl;
            break;
        }
    };
    void e_Hurt()
    {
        cVel->SetVelocityMultiplier(knockBack);

        switch (curWeapon)
        {
        case NOWEAPON:
            cAnim->SetAnimationState(AnimationState::Idle);
            break;
        case SWORD:
            //cAnim->SetAnimationState(AnimationState::SwordHurt);
            break;
        case BOW:
            cAnim->SetAnimationState(AnimationState::BowHurt);
            break;
        case SPEAR:
            //cAnim->SetAnimationState(AnimationState::SpearHurt);
            break;
        default:
            std::cout << "Unknown Weapon Type." << std::endl;
            break;
        }
    };
    void e_Dead()
    {
        cVel->SetVelocityMultiplier(idleSpeed);

        switch (curWeapon)
        {
        case NOWEAPON:
            cAnim->SetAnimationState(AnimationState::None);
            break;
        case SWORD:
            //cAnim->SetAnimationState(AnimationState::SwordDead);
            break;
        case BOW:
            cAnim->SetAnimationState(AnimationState::BowDead);
            break;
        case SPEAR:
            //cAnim->SetAnimationState(AnimationState::SpearDead);
            break;
        default:
            std::cout << "Unknown Weapon Type." << std::endl;
            break;
        }
    };
    void e_Crouch()
    {
        switch (curWeapon)
        {
        case NOWEAPON:
            cVel->SetVelocityMultiplier(bowOutSpeed);
            cAnim->SetAnimationState(AnimationState::Walk);
            break;
        case SWORD:
            cVel->SetVelocityMultiplier(bowOutSpeed);
            //cAnim->SetAnimationState(AnimationState::SwordCrouch);
            break;
        case BOW:
            cVel->SetVelocityMultiplier(bowOutSpeed);
            cAnim->SetAnimationState(AnimationState::BowCrouch);
            break;
        case SPEAR:
            cVel->SetVelocityMultiplier(bowOutSpeed);
            //cAnim->SetAnimationState(AnimationState::SpearCrouch);
            break;
        default:
            std::cout << "Unknown Weapon Type." << std::endl;
            break;
        }
    };
    void e_Lunge()
    {
        switch (curWeapon)
        {
        case NOWEAPON:
            //Entity specific stuff i.e. slime lungeing

            break;
        case SWORD:
            //cVel->SetVelocityMultiplier(SwordOutSpeed * 2);
            //cAnim->SetAnimationState(AnimationState::SwordLunge);
            break;
        case BOW:
            cVel->SetVelocityMultiplier(bowOutSpeed * 2);
            cAnim->SetAnimationState(AnimationState::BowLunge);
            break;
        case SPEAR:
            //cVel->SetVelocityMultiplier(spearOutSpeed * 2);
            //cAnim->SetAnimationState(AnimationState::SpearLunge);
            break;
        default:
            std::cout << "Unknown Weapon Type." << std::endl;
            break;
        }
    };
    void e_Attack()
    {
        switch (curWeapon)
        {
        case NOWEAPON:
            //Entity specific stuff i.e. slime lungeing

            break;
        case SWORD:
            //cVel->SetVelocityMultiplier(SwordOutSpeed / 2);
            //cAnim->SetAnimationState(AnimationState::SwordAttack);
            break;
        case BOW:
            cVel->SetVelocityMultiplier(bowOutSpeed / 2);
            cAnim->SetAnimationState(AnimationState::BowAttack);
            break;
        case SPEAR:
            //cVel->SetVelocityMultiplier(spearOutSpeed / 2);
            //cAnim->SetAnimationState(AnimationState::SpearAttack);
            break;
        default:
            std::cout << "Unknown Weapon Type." << std::endl;
            break;
        }
    };
    void e_Interact()
    {
        //Do something
    };



    std::shared_ptr<C_Animation> cAnim;
    std::shared_ptr<C_Velocity> cVel;

    float idleSpeed = 0.f, walkSpeed = 0.5f, runSpeed = 1.f, drawSpeed = 0.1f, bowOutSpeed = 0.25f, knockBack = -1.f;

    WeaponType curWeapon;
    WeaponType equippedWeapon;

};


#endif //C_Observer_h