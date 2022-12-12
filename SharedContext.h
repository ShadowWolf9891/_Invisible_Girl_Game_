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

static std::unordered_map<std::string, Event::DescriptorType> const strToEvent =
{
	{"None",E_None::descriptor},
	{"Idle",E_Idle::descriptor},
	{"Walk",E_Walk::descriptor},
	{"Run",E_Run::descriptor},
	{"BowDraw",E_BowDraw::descriptor},
	{"BowSheath",E_BowSheath::descriptor},
	{"BowParry",E_BowParry::descriptor},
	{"BowDodge",E_BowDodge::descriptor},
	{"BowHurt",E_BowHurt::descriptor},
	{"BowDead",E_BowDead::descriptor},
	{"BowIdle",E_BowIdle::descriptor},
	{"BowMove",E_BowMove::descriptor},
	{"BowCrouch",E_BowCrouch::descriptor},
	{"BowRetreat",E_BowRetreat::descriptor},
	{"BowLunge",E_BowLunge::descriptor},
	{"BowShootUp",E_BowShootUp::descriptor},
	{"BowShootStraight",E_BowShootStraight::descriptor},
	{"Interact",E_Interact::descriptor}

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
	{19, "C_WalkInLine"}
};


#endif /* SharedContext_h */