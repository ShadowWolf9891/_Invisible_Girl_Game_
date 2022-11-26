#pragma once

#ifndef C_Dialogue_h
#define C_Dialogue_h

#include "Component.h"
#include "Quest.h"


class C_Dialogue : public Component
{
public:
	C_Dialogue(Object* owner) : Component(owner) {};
	~C_Dialogue() = default;



	std::string GetDialogue(std::shared_ptr<Quest> currentQuest) 
	{
		StatusType questStatus = currentQuest->GetStatus();




	}
	void LoadDialogue() 
	{

	}



private:


};

#endif // C_Dialogue_h
