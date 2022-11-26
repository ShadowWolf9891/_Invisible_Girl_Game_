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
#include "Dispatcher.h"
#include "BT_ActionNodes.h"
#include "E_FireProjectile.h"
#include "XML_Parser.h"
#include "S_QuestManager.h"


class SceneGame : public Scene
{
public:
	// Constructor
	SceneGame(WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window, ResourceAllocator<sf::Font>& fontAllocator);
	
	void OnCreate() override;
	void OnDestroy() override;

	void ProcessInput() override;
	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
	void Draw(Window& window) override;

private:

	std::vector<std::shared_ptr<Object>> CreateObjectFromFile(std::string fileName, sf::Vector2f location);
	void CreateEnemy();

	void DispatchEvents(std::shared_ptr<C_Observer> ob);
	void AttachComponent(std::shared_ptr<Object> o, std::string _type, std::unordered_map<std::string, std::string> propertiesMap);

	WorkingDirectory& workingDir;
	Input input;
	ResourceAllocator<sf::Texture>& textureAllocator;
	ObjectCollection objects;
	TileMapParser mapParser;
	Window& window;
	SharedContext context;
	S_Drawable drawbleSystem;
	S_Collidable collisionSystem;
	S_QuestManager questSystem;
	Quadtree collisionTree;
	Raycast raycast;
	ResourceAllocator<sf::Font>& fontAllocator;
	Dispatcher dispatcher;
	XML_Parser xml_parser;

	BT::BehaviorTreeFactory bt_factory;
};


#endif /* SceneGame_h */


