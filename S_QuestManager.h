#pragma once

#ifndef S_QuestManager_h
#define S_QuestManager_h

#include <map>
#include "Quest.h"

class S_QuestManager 
{
public:
	void Add(std::shared_ptr<Quest> q) {_newQuests.insert(std::make_pair(q->GetID(), q)); };

	void ProcessNewQuests();
	
	void SetQuestAvailable(int questID);
	void SetQuestCompleted(int questID);
	void SetQuestFailed(int questID);
	void SetActiveQuest(int questID);
	std::shared_ptr<Quest> GetActiveQuest();

private:
	std::shared_ptr<Quest> _activeQuest;
	std::map<int, std::shared_ptr<Quest>> _allQuests;
	std::map<int, std::shared_ptr<Quest>>  _newQuests;
};



#endif // S_QuestManager_h
