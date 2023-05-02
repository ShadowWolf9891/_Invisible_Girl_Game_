#pragma once

#ifndef C_Weapon_h
#define C_Weapon_h

#include "Component.h"
#include "Weapon.h"


class C_Weapon : public Component
{
public:
	C_Weapon(Object* owner) : Component(owner), currentWeapon(std::make_shared<Weapon>()), previousWeapon(currentWeapon)
	{

	};
	virtual ~C_Weapon() = default;

	//Returns the previously equipped weapon for return to inventory
	std::shared_ptr<Weapon> EquipWeapon(std::shared_ptr<Weapon> w)
	{
		previousWeapon = currentWeapon;
		currentWeapon = w;
		return previousWeapon;
	};
	
	bool Attack()
	{



	};

private:
	
	std::shared_ptr<Weapon> currentWeapon;
	std::shared_ptr<Weapon> previousWeapon;


};


#endif // C_Weapon_h
