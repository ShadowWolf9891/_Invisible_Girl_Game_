#pragma once

#ifndef XML_Parser_h
#define XML_Parser_h

#include <map>

#include "tinyxml2.h"
#include "SharedContext.h"
#include "C_Animation.h"
#include "C_Dialogue.h"
#include "UI_Lists.h"
#include "UI_TextBox.h"
#include "Quest.h"

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

	std::unordered_map<int, std::unordered_map<StatusType, std::vector<std::shared_ptr<DialogueNode>>>> LoadDialogueDataFromFile(const std::string& filename);

	std::unordered_map<std::string, UIData> LoadUIFromFile(const std::string& filename);

	SharedContext& context;
	std::list<std::unique_ptr<tinyxml2::XMLDocument>> opened_documents;

private:
	void ParseAnimDoc(tinyxml2::XMLDocument* doc);
	void ParseInitDoc(tinyxml2::XMLDocument* doc);
	void ParseDialogueDoc(tinyxml2::XMLDocument* doc);
	void ParseUIFormatDoc(tinyxml2::XMLDocument* doc);
	std::shared_ptr <BaseUserInterface> CheckUIObjectType(const char* name, const tinyxml2::XMLElement* UIElement);

	template<class T> std::shared_ptr<T> CreateUIObject(const tinyxml2::XMLElement* element);

	std::unordered_map<AnimationState, AnimationList> animations;
	std::unordered_map<std::string, ObjectData> objects;
	std::unordered_map<int, std::unordered_map<StatusType, std::vector<std::shared_ptr<DialogueNode>>>> dNodes;
	std::unordered_map<std::string, UIData> uiObjects;

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

static std::unordered_map<std::string, StatusType> const strToQuestStatus =
{
	{"UNAVAILABLE", StatusType::UNAVAILABLE},
	{"AVAILABLE", StatusType::AVAILABLE},
	{"INPROGRESS", StatusType::INPROGRESS},
	{"COMPLETED", StatusType::COMPLETED},
	{"FINISHED", StatusType::FINISHED},
	{"FAILED", StatusType::FAILED}
};

#endif //XML_Parser_h