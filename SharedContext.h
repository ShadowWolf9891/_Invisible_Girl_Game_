#pragma once
#ifndef SharedContext_h
#define SharedContext_h

#include "Input.h"
#include "WorkingDirectory.h"
#include "ResourceAllocator.h"
#include "Window.h"
#include "Dispatcher.h"
#include "Event.h"
#include "behaviortree_cpp/bt_factory.h"
#include "Component.h"

class ObjectCollection;
class Raycast;
class Quadtree;
class Input;
class Dispatcher;
class XML_Parser;

struct SharedContext
{
	Input* input;
	ObjectCollection* objects;
	WorkingDirectory* workingDir;
	ResourceAllocator<sf::Texture>* textureAllocator;
	Window* window;
	Raycast* raycast;
	ResourceAllocator<sf::Font>* fontAllocator;
	Quadtree* collisionTree;
	Dispatcher* dispatcher;
	BT::BehaviorTreeFactory* bt_factory;
	XML_Parser* xml_parser;
};

struct UIData
{
	std::string name;
	std::string uiType;
	std::string parentName;
	std::vector<std::string> childrenNames;
	std::unordered_map<std::string, std::string> properties;
};

enum WeaponType
{
	NOWEAPON = 0,
	SWORD = 1,
	BOW = 2,
	SPEAR = 3
};

enum RarityValue
{
	NORARITY = 0,
	COMMON = 1,
	UNCOMMON = 2,
	RARE = 3,
	VERYRARE = 4,
	LEGENDARY = 5,
	MYTHICAL = 6
};

enum SpecialEffect
{
	NOEFFECT = 0,

	//Elemental Effects items can have

	FIRE = 1,	
	ICE = 2,
	LIGHTNING = 3,
	POISON = 4,

	//Status Effects 

	STUN = 5,
	KNOCKBACK = 6,
	BLEED = 7,
	FEAR = 8,
	SLEEP = 9,
	PIERCE = 10,

	//Self buffs

	PHYSATTACKUP = 11,
	MAGATTACKUP = 12,
	PHYSDEFENCEUP = 13,
	MAGDEFENCEUP = 14,
	AGILITYUP = 15,
	LIFESTEAL = 16		//Might not add if too OP

};

enum SkillType
{
	NOSKILL = 0,
	SWORDSKILL = 1,
	BOWSKILL = 2,
	SPEARSKILL = 3,
	MAGICSKILL = 4,
	HEAVYARMOURSKILL = 5,
	LIGHTARMOURSKILL = 6,
	FORGINGSKILL = 7,
	MININGSKILL = 8,
	CHOPPINGSKILL = 9,
	FARMINGSKILL = 10,
	COOKINGSKILL = 11,
	ARCANASKILL = 12,	//For enchanting or other stuff
	LORESKILL = 13,
	EXPLORESKILL = 14
};

static std::unordered_map<std::string, Event::DescriptorType> const strToEvent =
{
	{"None",E_None::descriptor},
	{"Idle",E_Idle::descriptor},
	{"Walk",E_Walk::descriptor},
	{"Run",E_Run::descriptor},
	{"DrawWeapon",E_DrawWeapon::descriptor},
	{"Sheath",E_Sheath::descriptor},
	{"Parry",E_Parry::descriptor},
	{"Dodge",E_Dodge::descriptor},
	{"Hurt",E_Hurt::descriptor},
	{"Dead",E_Dead::descriptor},
	{"Crouch",E_Crouch::descriptor},
	{"Retreat",E_Retreat::descriptor},
	{"Lunge",E_Lunge::descriptor},
	{"Attack",E_Attack::descriptor},
	{"ShootStraight",E_ShootStraight::descriptor},
	{"Interact",E_Interact::descriptor},
	{"LeftClick",E_LeftClick::descriptor},
	{"RightClick",E_RightClick::descriptor},
	{"MiddleClick",E_MiddleClick::descriptor},
	{"PauseMovement",E_PauseMovement::descriptor},
	{"UnPauseMovement",E_UnPauseMovement::descriptor}
};

static std::unordered_map<std::string, Input::Key> const strToKey =
{
	{"None",Input::Key::None},
	{"Left",Input::Key::Left},
	{"Right",Input::Key::Right},
	{"Up",Input::Key::Up},
	{"Down",Input::Key::Down},
	{"Esc",Input::Key::Esc},
	{"LBracket",Input::Key::LBracket},
	{"RBracket",Input::Key::RBracket},
	{"E",Input::Key::E},
	{"R",Input::Key::R},
	{"Shift",Input::Key::Shift}
};

static std::unordered_map<std::string, Input::Mouse> const strToMouse =
{
	{"None",Input::Mouse::None},
	{"Left",Input::Mouse::Left},
	{"Right",Input::Mouse::Right},
	{"Middle",Input::Mouse::Middle}
};

static std::unordered_map<int, std::string> const bitPosToString =
{
	{0, "None"},
	{1, "C_Animation"},
	{2, "C_BehaviourApplier"},
	{3, "C_BoxCollider"},
	{4, "C_Camera"},
	{5, "C_Direction"},
	{6, "C_Events"},
	{7, "C_InteractableTalking"},
	{8, "C_InteractWithObjects"},
	{9, "C_KeyboardMovement"},
	{10, "C_Mass"},
	{11, "C_Observer"},
	{12, "C_ProjectileAttack"},
	{13, "C_RemoveObjectOnCollisionEnter"},
	{14, "C_Sprite"},
	{15, "C_SteeringBehaviorChase"},
	{16, "C_SteeringBehaviorWallAvoidance"},
	{17, "C_Tree"},
	{18, "C_Velocity"},
	{19, "C_WalkInLine"},
	{20, "C_Dialogue"},
	{21, "C_FollowMouse"}
};


#endif /* SharedContext_h */