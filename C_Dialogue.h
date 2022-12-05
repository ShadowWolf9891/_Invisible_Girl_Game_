#pragma once

#ifndef C_Dialogue_h
#define C_Dialogue_h

#include <unordered_map>
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

	std::string GetDialogueText(std::shared_ptr<Quest> currentQuest)
	{
		StatusType questStatus = currentQuest->GetStatus();

		auto it = allDialogue.find(currentQuest->GetID());
		if (it != allDialogue.end()) 
		{
			auto it2 = it->second.find(currentQuest->GetStatus());
			if (it2 != it->second.end())
			{
				for (auto& dialogNode : it2->second)
				{
					if (dialogNode->ID == curNodeID)
					{
						return dialogNode->text;
					}
				}
			}
		}
	};
	
	std::unordered_map<int, std::unordered_map<StatusType, std::vector<std::shared_ptr<DialogueNode>>>> allDialogue;

private:
	
	int curNodeID, nextNodeID;
	std::shared_ptr<DialogueNode> curNode;
};

#endif // C_Dialogue_h
