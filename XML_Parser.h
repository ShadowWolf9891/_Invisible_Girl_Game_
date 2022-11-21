#pragma once

#ifndef XML_Parser_h
#define XML_Parser_h

#include <map>

#include "tinyxml2.h"
#include "SharedContext.h"
#include "C_Animation.h"

struct ObjectData 
{
	Bitmask componentMask;
	std::unordered_map<std::string, std::string> properties;
};

class XML_Parser
{
public:

	XML_Parser(SharedContext& context);

	std::unordered_map<AnimationState, AnimationList> LoadAnimFromFile(const std::string& filename);

	std::unordered_map <std::string, ObjectData> LoadInitialObjectDataFromFile(const std::string& filename);

	SharedContext& context;
	std::list<std::unique_ptr<tinyxml2::XMLDocument>> opened_documents;

private:
	void ParseAnimDoc(tinyxml2::XMLDocument* doc);
	void ParseInitDoc(tinyxml2::XMLDocument* doc);

	std::unordered_map<AnimationState, AnimationList> animations;
	std::unordered_map <std::string, ObjectData> objects;
};


//Static map of string / AnimationState pairs, where AnimationState is an Enum declared in C_Animation
static std::unordered_map<std::string, AnimationState> const strToAnimState = 
	{	
		{"None",AnimationState::None},
		{"Idle",AnimationState::Idle}, 
		{"Walk",AnimationState::Walk},
		{"Run",AnimationState::Run},
		{"BowDraw",AnimationState::BowDraw},
		{"BowSheath",AnimationState::BowSheath},
		{"BowParry",AnimationState::BowParry},
		{"BowDodge",AnimationState::BowDodge},
		{"BowHurt",AnimationState::BowHurt},
		{"BowDead",AnimationState::BowDead},
		{"BowIdle",AnimationState::BowIdle},
		{"BowMove",AnimationState::BowMove},
		{"BowCrouch",AnimationState::BowCrouch},
		{"BowRetreat",AnimationState::BowRetreat},
		{"BowLunge",AnimationState::BowLunge},
		{"BowShootUp",AnimationState::BowShootUp},
		{"BowShootStraight",AnimationState::BowShootStraight}
	};
static std::unordered_map<std::string, FacingDirection> const strToDir =
{
	{"None",FacingDirection::None},
	{"Down",FacingDirection::Down},
	{"Up",FacingDirection::Up},
	{"Right",FacingDirection::Right},
	{"Left",FacingDirection::Left}
};

#endif // XML_Parser_h
