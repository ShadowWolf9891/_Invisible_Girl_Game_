#pragma once

#ifndef Weapon_h
#define Weapon_h

#include "SharedContext.h"

class Weapon
{
public:
	Weapon(): _ID(0), _iconID(0), _name(""), _description(""), _wType(WeaponType::NOWEAPON), _rarity(RarityValue::NORARITY),
		_effect(SpecialEffect::NOEFFECT), _requiredSkillType(SkillType::NOSKILL), _baseDamage(1), _critChance(0.0f), _range(32),
		_attackSpeed(1.0f), _requiredSkillLevel(0){};
	~Weapon() {};

	bool CreateWeapon(std::unordered_map<std::string, std::string> propertiesMap)
	{
		try
		{
			_ID = atoi(propertiesMap.at("ID").c_str());
			_iconID = atoi(propertiesMap.at("iconID").c_str());
			_name = propertiesMap.at("name").c_str();
			_description = propertiesMap.at("description").c_str();
			_wType = WeaponType(atoi(propertiesMap.at("weaponType").c_str()));
			_rarity = RarityValue(atoi(propertiesMap.at("rarity").c_str()));
			_effect = SpecialEffect(atoi(propertiesMap.at("effect").c_str()));
			_requiredSkillType = SkillType(atoi(propertiesMap.at("requiredSkill").c_str()));
			_baseDamage = atof(propertiesMap.at("baseDamage").c_str());
			_critChance = atof(propertiesMap.at("critChance").c_str());
			_range = atof(propertiesMap.at("range").c_str());
			_attackSpeed = atof(propertiesMap.at("attackSpeed").c_str());
			_requiredSkillLevel = atoi(propertiesMap.at("requiredSkillLevel").c_str());
		}
		catch (const std::exception& e) {
			std::cerr << "Error creating weapon: " << e.what() << std::endl;
			return false;
		}

		return true;

	};

	int GetID() { return _ID; };
	int GetIconID() { return _iconID; };
	std::string GetName() { return _name; };
	std::string GetDescription() { return _description; };
	WeaponType GetWeaponType() { return _wType; };
	RarityValue GetRarity() { return _rarity; };
	SpecialEffect GetEffect() {return _effect; };
	SkillType GetRequiredSkillType() { return _requiredSkillType; };
	float GetBaseDamage() { return _baseDamage; };
	float GetCritChance() { return _critChance; };
	float GetRange() { return _range; };
	float GetAttackSpeed() { return _attackSpeed; };
	int GetRequiredSkillLevel() { return _requiredSkillLevel; };


private:

	int _ID;
	int _iconID;
	std::string _name;
	std::string _description;
	WeaponType _wType;
	RarityValue _rarity;
	SpecialEffect _effect;
	SkillType _requiredSkillType;

	float _baseDamage;
	float _critChance; //0 to 1 where 1 is 100%
	float _range;
	float _attackSpeed;
	int _requiredSkillLevel;



};
#endif // Weapon_h
