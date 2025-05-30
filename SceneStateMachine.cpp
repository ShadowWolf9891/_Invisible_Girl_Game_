#include "SceneStateMachine.h"
SceneStateMachine::SceneStateMachine() : scenes(0), curScene(0), insertedSceneID(0) 
{

}
void SceneStateMachine::ProcessInput()
{
	if (curScene)
	{
		curScene->ProcessInput();
	}
}
void SceneStateMachine::Update(float deltaTime)
{
	if (curScene)
	{
		curScene->Update(deltaTime);
	}
}
void SceneStateMachine::LateUpdate(float deltaTime)
{
	if (curScene)
	{
		curScene->LateUpdate(deltaTime);
	}

	if (curScene->switchScene)
	{
		SwitchTo();
	}
}
void SceneStateMachine::Draw(Window& window)
{
	if (curScene)
	{
		curScene->Draw(window);
	}
}
unsigned int SceneStateMachine::Add(std::shared_ptr<Scene> scene)
{
	auto inserted = scenes.insert(std::make_pair(insertedSceneID, scene));
	if (inserted.second)
	{
		scene->OnCreate();
		insertedSceneID++;
	}
	
	return inserted.first->first;
}
void SceneStateMachine::Remove(unsigned int id)
{
	auto it = scenes.find(id);
	if (it != scenes.end())
	{
		if (curScene == it->second)
		{
			// If the scene we are removing is the current scene, 
			// we also want to set that to a null pointer so the scene 
			// is no longer updated.
			curScene = nullptr;
		}
		// We make sure to call the OnDestroy method 
		// of the scene we are removing.
		it->second->OnDestroy();
		scenes.erase(it);
	}
}
void SceneStateMachine::SwitchTo()
{
	if (curScene)
	{
		auto it = scenes.find(curScene->switchToSceneID);
		if (it != scenes.end())
		{
			// If we have a current scene, we call its OnDeactivate method.
			curScene->OnDeactivate();
			// Setting the current scene ensures that it is updated and drawn.
			curScene = it->second;
			curScene->OnActivate();
		}
	}
}
void SceneStateMachine::SwitchTo(unsigned int id)
{
	auto it = scenes.find(id);
	if (it != scenes.end())
	{
		if (curScene)
		{
			// If we have a current scene, we call its OnDeactivate method.
			curScene->OnDeactivate();
		}
		// Setting the current scene ensures that it is updated and drawn.
		curScene = it->second;
		curScene->OnActivate();
	}
}