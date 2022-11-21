#include "SceneGame.h"

SceneGame::SceneGame(WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window, ResourceAllocator<sf::Font>& fontAllocator) : 
    workingDir(workingDir), textureAllocator(textureAllocator), mapParser(textureAllocator, context), window(window), 
    objects(drawbleSystem, collisionSystem), collisionTree(5, 5, 0, { sf::Vector2f(-8, -8), sf::Vector2f(2048, 2048) }, nullptr), collisionSystem(collisionTree),
    raycast(collisionTree), fontAllocator(fontAllocator), xml_parser(context)
{ 

}

void SceneGame::OnCreate()
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

    bt_factory.registerNodeType<IsOnGround>("IsOnGround");
    bt_factory.registerNodeType<GetMoveSpeed>("GetMoveSpeed");

    bt_factory.registerNodeType<TriggerEvent>("TriggerEvent");

    bt_factory.registerNodeType<IsKeyPressed>("IsKeyPressed");
    bt_factory.registerNodeType<IsKeyPressed>("IsKeyDown");
    bt_factory.registerNodeType<IsKeyPressed>("IsKeyUp");
    

    using std::filesystem::directory_iterator;
    for (auto const& entry : directory_iterator("../data/trees/"))
    {
        if (entry.path().extension() == ".xml")
        {
            bt_factory.registerBehaviorTreeFromFile(entry.path().string());
        }
    }

    
    //CreateEnemy();
    
    std::vector<std::shared_ptr<Object>> levelTiles = mapParser.Parse(workingDir.Get() + "level_data/Town_1_data.json");
    objects.Add(levelTiles);
    
    std::vector<std::shared_ptr<Object>> playerList = CreateObjectFromFile("data/obj/Player_Data.xml", sf::Vector2f(1024, 1024));
    objects.Add(playerList);
    std::vector<std::shared_ptr<Object>> NPCList = CreateObjectFromFile("data/obj/PassiveNPC_Data.xml", sf::Vector2f(1024, 1224));
    objects.Add(NPCList);
    
}

//Creates a new object from XML file and places it at location
std::vector<std::shared_ptr<Object>> SceneGame::CreateObjectFromFile(std::string fileName, sf::Vector2f location)
{
    std::vector<std::shared_ptr<Object>> objectList; 
    auto objectData = xml_parser.LoadInitialObjectDataFromFile(workingDir.Get() + fileName);
    for (auto& object : objectData)
    {
        std::shared_ptr <Object> obj = std::make_shared<Object>(&context);
        Bitmask bm = object.second.componentMask;
        std::unordered_map<std::string,std::string> propertiesMap = object.second.properties;
        
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

}
void SceneGame::CreateEnemy()
{
    std::shared_ptr<Object> npc = std::make_shared<Object>(&context);
    npc->tag->Set(Tag::NPC);
    npc->transform->SetPosition(700, 700);

    auto sprite = npc->AddComponent<C_Sprite>();
    sprite->SetDrawLayer(DrawLayer::Entities);

    const int textureID = textureAllocator.Add(workingDir.Get() + "Assets/Character/20.01a - Character Base 2.4a (comp. v01)/char_a_p1/char_a_p1_0bas_demn_v02.png");

    auto collider = npc->AddComponent<C_BoxCollider>();
    collider->SetSize(16, 32);
    collider->SetOffset(0.f, 0.f);
    collider->SetLayer(CollisionLayer::NPC);

    npc->AddComponent<C_Velocity>();
    npc->AddComponent<C_Mass>();
    npc->AddComponent<C_Direction>();

    npc->AddComponent<C_BehaviourApplier>();
    auto chase = npc->AddComponent<C_SteeringBehaviourChase>();
    chase->SetTarget(Tag::Player);
    npc->AddComponent<C_SteeringBehaviourWallAvoidance>();

    objects.Add(npc);


}
void SceneGame::DispatchEvents(std::shared_ptr<C_Observer> ob)
{
    for (auto& e : strToEvent)
    {
        dispatcher.subscribe(e.second, std::bind(&C_Observer::handle, ob, std::placeholders::_1));
    }
}
void SceneGame::OnDestroy()
{

}

void SceneGame::ProcessInput()
{
	input.Update();
}

void SceneGame::Update(float deltaTime)
{
    objects.ProcessRemovals();
    objects.ProcessNewObjects();
    objects.Update(deltaTime);

    Debug::HandleCameraZoom(window, input); //Only works for this scene at the moment

}
void SceneGame::LateUpdate(float deltaTime)
{
    objects.LateUpdate(deltaTime);
}

void SceneGame::Draw(Window& window)
{
   objects.Draw(window);
   Debug::Draw(window);
}

void SceneGame::AttachComponent(std::shared_ptr<Object> o, std::string _type, std::unordered_map<std::string, std::string> propertiesMap)
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
        textureAllocator.Add(workingDir.Get() + propertiesMap.at("BaseTextureFile"));
       
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


    //Add any new components here.



}
