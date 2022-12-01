#pragma once

#ifndef C_Dialogue_h
#define C_Dialogue_h

#include "Component.h"
#include "Quest.h"

struct OptionNode
{
	int nextNodeID;
	std::string text;
	StatusType returnCode;
};

struct DialogueNode
{
	int ID;
	std::string text;
	std::vector<std::shared_ptr<OptionNode>> options;
};


class C_Dialogue : public Component
{
public:
	C_Dialogue(Object* owner) : Component(owner) {};
	~C_Dialogue() = default;



	std::string GetDialogue(std::shared_ptr<Quest> currentQuest) 
	{
		StatusType questStatus = currentQuest->GetStatus();

	}
	void LoadDialogue(int curQuestID, StatusType curQuestStatus)
	{



	};



private:
	
	

};

#endif // C_Dialogue_h
