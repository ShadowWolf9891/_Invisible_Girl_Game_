#include "S_QuestManager.h"

void S_QuestManager::ProcessNewQuests()
{
	if (_newQuests.size() > 0) 
	{
		for (auto& q : _newQuests)
		{
			auto it = _allQuests.find(q.first);
			if (it == _allQuests.end()) 
			{
				_allQuests.insert(q);
			}
		}
		_newQuests.clear();
	}
}
void S_QuestManager::SetQuestAvailable(int questID)
{
	auto it = _allQuests.find(questID);
	if (it == _allQuests.end() || it->second->GetStatus() != UNAVAILABLE) return;

	it->second->SetStatus(AVAILABLE);
}
void S_QuestManager::SetQuestCompleted(int questID)
{
	auto it = _allQuests.find(questID);
	if (it == _allQuests.end()) return;

	it->second->SetStatus(COMPLETED);
}
void S_QuestManager::SetQuestFailed(int questID)
{
	auto it = _allQuests.find(questID);
	if (it == _allQuests.end()) return;

	it->second->SetStatus(FAILED);
}
void S_QuestManager::SetActiveQuest(int questID)
{
	auto it = _allQuests.find(questID);
	if (it == _allQuests.end()) return;

	if (it->second->GetStatus() == AVAILABLE) 
	{
		_activeQuest = it->second;
	}
}
std::shared_ptr<Quest> S_QuestManager::GetActiveQuest()
{
	return _activeQuest;
}
