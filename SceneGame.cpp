#include "SceneGame.h"

void SceneGame::OnCreate()
{
    Scene::OnCreate();

    //CreateEnemy();

    questSystem.Add(std::make_shared<Quest>(0, "Lost Coin"));
    questSystem.ProcessNewQuests();
    questSystem.SetQuestAvailable(0);
    questSystem.SetActiveQuest(0);
    
    std::vector<std::shared_ptr<Object>> levelTiles = mapParser.Parse(workingDir.Get() + "level_data/Town_1_data.json");
    objects.Add(levelTiles);
    
    std::vector<std::shared_ptr<Object>> itemList = CreateObjectFromFile("data/obj/Item_Data.xml", sf::Vector2f(1000, 1000));
    objects.Add(itemList);

    std::vector<std::shared_ptr<Object>> playerList = CreateObjectFromFile("data/obj/Player_Data.xml", sf::Vector2f(1024, 1024));
    objects.Add(playerList);
    std::vector<std::shared_ptr<Object>> NPCList = CreateObjectFromFile("data/obj/PassiveNPC_Data.xml", sf::Vector2f(1024, 1224));
    objects.Add(NPCList);

    std::vector<std::shared_ptr<Object>> mouseList = CreateObjectFromFile("data/obj/Mouse_Data.xml", sf::Vector2f(0, 0));
    objects.Add(mouseList);

    
}

//void SceneGame::CreateEnemy()
//{
//    std::shared_ptr<Object> npc = std::make_shared<Object>(&context);
//    npc->tag->Set(Tag::NPC);
//    npc->transform->SetPosition(700, 700);
//
//    auto sprite = npc->AddComponent<C_Sprite>();
//    sprite->SetDrawLayer(DrawLayer::Entities);
//
//    const int textureID = textureAllocator.Add(workingDir.Get() + "Assets/Character/20.01a - Character Base 2.4a (comp. v01)/char_a_p1/char_a_p1_0bas_demn_v02.png");
//
//    auto collider = npc->AddComponent<C_BoxCollider>();
//    collider->SetSize(16, 32);
//    collider->SetOffset(0.f, 0.f);
//    collider->SetLayer(CollisionLayer::NPC);
//
//    npc->AddComponent<C_Velocity>();
//    npc->AddComponent<C_Mass>();
//    npc->AddComponent<C_Direction>();
//
//    npc->AddComponent<C_BehaviourApplier>();
//    auto chase = npc->AddComponent<C_SteeringBehaviourChase>();
//    chase->SetTarget(Tag::Player);
//    npc->AddComponent<C_SteeringBehaviourWallAvoidance>();
//
//    objects.Add(npc);
//
//
//}

void SceneGame::OnDestroy()
{
    sf::View view = window.GetView();
    view.zoom(2);
    window.SetView(view);
}

void SceneGame::OnActivate()
{
    sf::View view = window.GetView();
    view.zoom(0.5f);
    window.SetView(view);
}

void SceneGame::ProcessInput()
{
    Scene::ProcessInput();
}

void SceneGame::Update(float deltaTime)
{
    Scene::Update(deltaTime);  //Processes objects 

    Debug::HandleCameraZoom(window, input); //Only works for this scene at the moment

}
void SceneGame::LateUpdate(float deltaTime)
{
    Scene::LateUpdate(deltaTime);
}

void SceneGame::Draw(Window& window)
{
   Scene::Draw(window); // Draws Objects
   Debug::Draw(window);
   
}
