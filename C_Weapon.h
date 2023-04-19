#pragma once

#ifndef C_Weapon_h
#define C_Weapon_h

#include "Component.h"
#include "Weapon.h"


class C_Weapon : public Component
{
public:
	C_Weapon(Object* owner) : Component(owner)
	{

	};
	virtual ~C_Weapon() = default;

	

private:
	
	WeaponType currentWeapon;



};


#endif // C_Weapon_h
