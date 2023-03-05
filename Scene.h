#pragma once
#ifndef Scene_h
#define Scene_h

#include "window.h"
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

class Scene
{
public:

    Scene(WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window, ResourceAllocator<sf::Font>& fontAllocator) :
        workingDir(workingDir), textureAllocator(textureAllocator), window(window), fontAllocator(fontAllocator),
        objects(drawbleSystem, collisionSystem), mapParser(textureAllocator, context), collisionTree(5, 5, 0, { sf::Vector2f(-8, -8), sf::Vector2f(2048, 2048) }, nullptr),
        collisionSystem(collisionTree), raycast(collisionTree), xml_parser(context), switchToSceneID(0), switchScene(false){};

	// Called when scene initially created. Called once.
    virtual void OnCreate()
    {
        InitializeContext();
        RegisterNodes();

        using std::filesystem::directory_iterator;
        for (auto const& entry : directory_iterator("../data/trees/"))
        {
            if (entry.path().extension() == ".xml")
            {
                bt_factory.registerBehaviorTreeFromFile(entry.path().string());
            }
        }
    };
	// Called when scene destroyed. Called at most once (if a scene 
	// is not removed from the game, this will never be called).
	virtual void OnDestroy() = 0;
	// Called whenever a scene is transitioned into. Can be 
	// called many times in a typical game cycle.
	virtual void OnActivate() {};
	// Called whenever a transition out of a scene occurs. 
	// Can be called many times in a typical game cycle.
	virtual void OnDeactivate() {};
	// The below functions can be overridden as necessary in our scenes.
	virtual void ProcessInput()
    {
        input.Update();
    };
	virtual void Update(float deltaTime)
    {
        objects.ProcessRemovals();
        objects.ProcessNewObjects();
        objects.Update(deltaTime);
    };
	virtual void LateUpdate(float deltaTime)
    {
        objects.LateUpdate(deltaTime);
    };
	virtual void Draw(Window& window)
    {
        objects.Draw(window);
    };

    virtual void SetSwitchToScene(unsigned int id)
    {
        switchToSceneID = id;
    };

    virtual void InitializeContext()
    {
        context.input = &input;
        context.objects = &objects;
        context.workingDir = &workingDir;
        context.textureAllocator = &textureAllocator;
        context.window = &window;
        context.raycast = &raycast;
        context.fontAllocator = &fontAllocator;
        context.collisionTree = &collisionTree;
        context.dispatcher = &dispatcher;
        context.bt_factory = &bt_factory;
        context.xml_parser = &xml_parser;
    };

    virtual void RegisterNodes()
    {
        bt_factory.registerNodeType<IsOnGround>("IsOnGround");
        bt_factory.registerNodeType<IsDoneAnimating>("IsDoneAnimating");
        bt_factory.registerNodeType<IsInteracting>("IsInteracting");
        bt_factory.registerNodeType<SetIsInteracting>("SetIsInteracting");
        bt_factory.registerNodeType<CheckInteraction>("CheckInteraction");
        bt_factory.registerNodeType<GetMoveSpeed>("GetMoveSpeed");

        bt_factory.registerNodeType<TriggerEvent>("TriggerEvent");

        bt_factory.registerNodeType<IsKeyPressed>("IsKeyPressed");
        bt_factory.registerNodeType<IsKeyDown>("IsKeyDown");
        bt_factory.registerNodeType<IsKeyUp>("IsKeyUp");

        bt_factory.registerNodeType<IsMousePressed>("IsMousePressed");
        bt_factory.registerNodeType<IsMouseDown>("IsMouseDown");
        bt_factory.registerNodeType<IsMouseUp>("IsMouseUp");

        enum bowInfo { NOBOW = 0, BOWDRAW = 1, BOWOUT = 2, BOWSHEATH = 3 };
        bt_factory.registerScriptingEnums<bowInfo>();
    };

    virtual std::vector<std::shared_ptr<Object>> CreateObjectFromFile(std::string fileName, sf::Vector2f location)
    {
        std::vector<std::shared_ptr<Object>> objectList;
        auto objectData = xml_parser.LoadInitialObjectDataFromFile(workingDir.Get() + fileName);
        for (auto& object : objectData)
        {
            std::shared_ptr <Object> obj = std::make_shared<Object>(&context);
            Bitmask bm = object.second.componentMask;
            std::unordered_map<std::string, std::string> propertiesMap = object.second.properties;

            for (int pos = 0; pos < bitPosToString.size(); pos++)
            {
                if (bm.GetBit(pos))
                {
                    auto it = bitPosToString.find(pos);
                    if (it != bitPosToString.end())
                    {
                        std::string cType = it->second;
                        AttachComponent(obj, cType, propertiesMap);

                        std::cout << "Attached component " << cType << " to object " << object.first << std::endl;
                    }
                }
            }
            obj->SetName(object.first);
            obj->transform->SetPosition(location);
            obj->tag->Set(Tag(atoi(propertiesMap.at("Tag").c_str())));

            objectList.emplace_back(obj);

        }

        return objectList;

    };

    virtual void AttachComponent(std::shared_ptr<Object> o, std::string _type, std::unordered_map<std::string, std::string> propertiesMap)
    {
        //Not very elegant but there does not appear to be another way to convert string to typename at the moment

        if (_type == "C_Animation")
        {
            auto animations = xml_parser.LoadAnimFromFile(workingDir.Get() + propertiesMap.at("AnimationFile"));
            auto animComponent = o->AddComponent<C_Animation>();
            for (auto& a : animations) {
                animComponent->AddAnimation(a.first, a.second);
            }
        }
        else if (_type == "C_BehaviourApplier") o->AddComponent<C_BehaviourApplier>();
        else if (_type == "C_BoxCollider")
        {
            auto collider = o->AddComponent<C_BoxCollider>();
            collider->SetSize(atoi(propertiesMap.at("BBoxSizeX").c_str()), atoi(propertiesMap.at("BBoxSizeY").c_str()));
            collider->SetOffset(atoi(propertiesMap.at("BBoxOffsetX").c_str()), atoi(propertiesMap.at("BBoxOffsetY").c_str()));
            collider->SetLayer(CollisionLayer(atoi(propertiesMap.at("BBoxCollisionLayer").c_str())));
        }
        else if (_type == "C_Camera") o->AddComponent<C_Camera>();
        else if (_type == "C_Direction") o->AddComponent<C_Direction>();
        else if (_type == "C_Events") o->AddComponent<C_Events>();
        else if (_type == "C_InteractableTalking") o->AddComponent<C_InteractableTalking>();
        else if (_type == "C_InteractWithObjects") o->AddComponent<C_InteractWithObjects>();
        else if (_type == "C_KeyboardMovement") o->AddComponent<C_KeyboardMovement>();
        else if (_type == "C_Mass") o->AddComponent<C_Mass>();
        else if (_type == "C_Observer")
        {
            auto classObserver = o->AddComponent<C_Observer>();
            DispatchEvents(classObserver);
        }
        else if (_type == "C_ProjectileAttack") o->AddComponent<C_ProjectileAttack>();
        else if (_type == "C_RemoveObjectOnCollisionEnter") o->AddComponent<C_RemoveObjectOnCollisionEnter>();
        else if (_type == "C_Sprite")
        {
            auto sprite = o->AddComponent<C_Sprite>();
            sprite->SetDrawLayer(DrawLayer(atoi(propertiesMap.at("DrawLayer").c_str())));
            int texID = textureAllocator.Add(workingDir.Get() + propertiesMap.at("BaseTextureFile"));
            sprite->Load(texID);
            sprite->SetTextureRect(atoi(propertiesMap.at("X").c_str()), atoi(propertiesMap.at("Y").c_str()),
                atoi(propertiesMap.at("Width").c_str()), atoi(propertiesMap.at("Height").c_str()));

            if (propertiesMap.contains("ScaleX") && propertiesMap.contains("ScaleY")) {
                sprite->SetScale(atof(propertiesMap.at("ScaleX").c_str()), atof(propertiesMap.at("ScaleY").c_str()));
            }
        }
        else if (_type == "C_SteeringBehaviourChase") o->AddComponent<C_SteeringBehaviourChase>();
        else if (_type == "C_SteeringBehaviourWallAvoidance") o->AddComponent<C_SteeringBehaviourWallAvoidance>();
        else if (_type == "C_Tree")
        {
            auto tree = o->AddComponent<C_Tree>();
            tree->bTree = bt_factory.createTree(propertiesMap.at("TreeName"));
        }
        else if (_type == "C_Velocity") o->AddComponent<C_Velocity>();
        else if (_type == "C_WalkInLine") o->AddComponent<C_WalkInLine>();
        else if (_type == "C_Dialogue")
        {
            //auto dialogue = xml_parser.LoadDialogueDataFromFile(workingDir.Get() + propertiesMap.at("DialogueFile"));
            //auto dComponent = o->AddComponent<C_Dialogue>();
            ////dComponent->LoadUIFormat(xml_parser.LoadUIFromFile(workingDir.Get() + "data/dialogue/DialogueUIFormat.xml"));
            //dComponent->SetCurQuest(questSystem.GetActiveQuest());
            //dComponent->SetDrawLayer(DrawLayer::UI);
            //dComponent->allDialogue = dialogue;
        }
        else if (_type == "C_FollowMouse") o->AddComponent<C_FollowMouse>();


        //Add any new components here.

    };

    virtual void DispatchEvents(std::shared_ptr<C_Observer> ob)
    {
        for (auto& e : strToEvent)
        {
            dispatcher.subscribe(e.second, std::bind(&C_Observer::handle, ob, std::placeholders::_1));
        }
    };

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
    unsigned int switchToSceneID;
    BT::BehaviorTreeFactory bt_factory;

    bool switchScene;
};

#endif /* Scene_h */