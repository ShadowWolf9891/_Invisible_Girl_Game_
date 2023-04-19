#pragma once

#ifndef Weapon_h
#define Weapon_h

#include "SharedContext.h"

class Weapon
{
public:
	Weapon() {};
	~Weapon() {};

	bool CreateWeapon(std::unordered_map<std::string, std::string> propertiesMap)
	{
		
	};

private:

	int _ID;
	std::string _name;
	std::string _description;
	WeaponType _wType;
	RarityValue _rarity;
	SpecialEffect _effect;

	float _baseDamage;
	float _critChance; //0 to 1 where 1 is 100%
	float _range;
	float _attackSpeed;
	float _requiredSkill;



};

#endif // Weapon_h
