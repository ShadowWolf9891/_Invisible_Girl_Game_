#pragma once
#ifndef SceneGame_h
#define SceneGame_h

#include "Scene.h"
#include "Input.h"
#include "WorkingDirectory.h"
#include "Object.h"
#include "C_Sprite.h"
#include "C_KeyboardMovement.h"
#include "C_Animation.h"
#include "C_BoxCollider.h"
#include "ResourceAllocator.h"
#include "ObjectCollection.h"
#include "TileMapParser.h"
#include "Debug.h"
#include "C_Camera.h"
#include "C_ProjectileAttack.h"
#include "C_Velocity.h"
#include "C_Direction.h"
#include "Raycast.h"
#include "C_InteractWithObjects.h"
#include "C_InteractableTalking.h"
#include "C_WalkInLine.h"
#include "C_BehaviourApplier.h"
#include "C_SteeringBehaviourChase.h"
#include "C_SteeringBehaviourWallAvoidance.h"
#include "C_Observer.h"
#include "C_Tree.h"
#include "C_Dialogue.h"
#include "C_FollowMouse.h"
#include "Dispatcher.h"
#include "BT_ActionNodes.h"
#include "E_FireProjectile.h"
#include "XML_Parser.h"
#include "S_QuestManager.h"
#include "Weapon.h"
#include "C_Weapon.h"


class SceneGame : public Scene
{
public:
	// Constructor
	SceneGame(WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window, ResourceAllocator<sf::Font>& fontAllocator) :
		Scene(workingDir, textureAllocator, window, fontAllocator), showMenu(false), showQuitMenu(false) {};
	
	void OnCreate() override;
	void OnDestroy() override;
	void OnActivate() override;
	void OnDeactivate() override;

	void ProcessInput() override;
	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
	void Draw(Window& window) override;

private:

	void DrawMenu();
	void DrawQuitMenu();

	bool showMenu, showQuitMenu;
	sf::Texture* buttonTexture;
	ImGuiWindowFlags window_flags;  //Specifies info about the window

	std::map<int, Weapon> weapons;
	
};


#endif /* SceneGame_h */


