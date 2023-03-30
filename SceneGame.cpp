#include "SceneGame.h"

void SceneGame::OnCreate()
{
    Scene::OnCreate();

    //CreateEnemy();

    int buttonID = textureAllocator.Add(workingDir.Get() + "Assets/UI/ButtonImage.png");

    buttonTexture = textureAllocator.Get(buttonID).get();


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

    std::vector<std::shared_ptr<Object>> EnemyList = CreateObjectFromFile("data/obj/HostileNPC_Data.xml", sf::Vector2f(1224, 1224));
    objects.Add(EnemyList);

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

void SceneGame::OnDeactivate()
{
    Scene::OnDeactivate();
    showQuitMenu = false;
    showMenu = false;
}

void SceneGame::ProcessInput()
{
    Scene::ProcessInput();
    
    if (input.IsKeyDown(Input::Key::Tab)){showMenu = !showMenu;} //Invert the showMenu if tab is pressed
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
   DrawMenu();
   DrawQuitMenu();
   

}

void SceneGame::DrawMenu()
{
    if (!showMenu) return;

    window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

    static int inventoryClicked = 0, equipClicked = 0, partyClicked = 0, saveClicked = 0, loadClicked = 0, optionsClicked = 0, exitClicked = 0;

    
    ImGui::SetNextWindowSize(ImVec2(window.GetRenderWindow().getSize().x * 0.2, window.GetRenderWindow().getSize().y * 0.8));
    ImGui::SetNextWindowPos(ImVec2((window.GetRenderWindow().getSize().x - ImGui::GetWindowContentRegionMax().x), window.GetRenderWindow().getSize().y * 0.1));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, sf::Color(100, 60, 15, 250)); //Brownish

    if (ImGui::Begin("GameMenu", NULL, window_flags))
    {
        if (CreateImageButton("btnInventory", "Inventory", *buttonTexture)) inventoryClicked++;
        if (CreateImageButton("btnEquip", "Equip", *buttonTexture)) equipClicked++;
        if (CreateImageButton("btnParty", "Party", *buttonTexture)) partyClicked++;
        if (CreateImageButton("btnSave", "Save", *buttonTexture)) saveClicked++;
        if (CreateImageButton("btnLoad", "Load", *buttonTexture)) loadClicked++;
        if (CreateImageButton("btnOptions", "Options", *buttonTexture)) optionsClicked++;
        if (CreateImageButton("btnQuit", "Exit", *buttonTexture)) exitClicked++;

        if (inventoryClicked & 1)
        {
            inventoryClicked = 0;
        }
        if (equipClicked & 1)
        {
            equipClicked = 0;
        }
        if (partyClicked & 1)
        {
            partyClicked = 0;
        }
        if (saveClicked & 1)
        {
            saveClicked = 0;
        }
        if (loadClicked & 1)
        {
            loadClicked = 0;
        }
        if (optionsClicked & 1)
        {
            optionsClicked = 0;
        }

        if (exitClicked & 1)
        {
            showQuitMenu = true;
            showMenu = false;
            exitClicked = 0;
        }
        

    };

    ImGui::End();

    ImGui::PopStyleColor();


}

void SceneGame::DrawQuitMenu()
{
    if (!showQuitMenu) return;

    window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

    static int yesClicked = 0, noClicked = 0;

    ImGui::SetNextWindowSize(ImVec2(window.GetRenderWindow().getSize().x, window.GetRenderWindow().getSize().y));
    ImGui::SetNextWindowPos(ImVec2(0,0));

    ImGui::PushStyleColor(ImGuiCol_WindowBg, sf::Color(0, 0, 0, 100)); //Black/Gray

    if (ImGui::Begin("QuitMenu", NULL, window_flags))
    {
        ImVec2 childWindowSize = ImVec2(ImGui::GetWindowSize().x * 0.4, ImGui::GetWindowSize().y * 0.2);
        ImVec2 parentWindowSize = ImGui::GetWindowContentRegionMax();
        ImVec2 childWindowPos = ImVec2((parentWindowSize.x - childWindowSize.x) * 0.5f, (parentWindowSize.y - childWindowSize.y) * 0.5f);

        ImGui::SetCursorPos(childWindowPos);
      
        if (ImGui::BeginChild("QuitMenuChild", childWindowSize, true, window_flags))
        {

            CreateImportantText("Are you sure you want to return to the main menu?", 1.0f);

            float spacing = CalculateSpacing(childWindowSize.x, GetScaledItemByWindowSize(buttonTexture->getSize(), ImVec2(0.1f, 0.1f)), 2);

            if (CreateImageButton("btnYes", "Yes", *buttonTexture)) yesClicked++;

            ImGui::SameLine();

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + spacing);

            if (CreateImageButton("btnNo", "No", *buttonTexture)) noClicked++;


            if (yesClicked & 1)
            {
                switchScene = true;
                showQuitMenu = false;
                yesClicked = 0;
            }
            if (noClicked & 1)
            {
                showQuitMenu = false;
                showMenu = true;
                noClicked = 0;
            }
        }
    }
    ImGui::EndChild();
    ImGui::End();

    ImGui::PopStyleColor();
}
