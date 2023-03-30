#pragma once
#ifndef SceneMainMenu_h
#define SceneMainMenu_h

#include "Input.h"
#include "GUI.h"
#include "imgui.h"
#include "Scene.h"

class SceneMainMenu : public Scene
{
public:
	SceneMainMenu(WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window, ResourceAllocator<sf::Font>& fontAllocator) : 
		Scene(workingDir, textureAllocator, window, fontAllocator), window_flags(0), p_open(false) {};
	
	
	void OnCreate() override
	{
		Scene::OnCreate();
		p_open = true;
		
		int buttonID = textureAllocator.Add(workingDir.Get() + "Assets/UI/ButtonImage.png");

		buttonTexture = textureAllocator.Get(buttonID).get(); //This gets the raw pointer. Make sure that the shared_ptr is not deleted before raw pointer.

		/*componentGrid.Load();

		this->background.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
		this->background.setFillColor(sf::Color::White);*/


		/*std::string btnName = "StartBtn";
		const int fontID = fontAllocator.Add(workingDir.Get() + "Assets/Fonts/joystix monospace.ttf");
		std::shared_ptr<sf::Font> font = fontAllocator.Get(fontID);

		GUI::Button startBtn(btnName, 100, 100, 100, 100, font, 16,
			sf::Color::Black, sf::Color::Black, sf::Color::Black, sf::Color::Blue, sf::Color::Cyan, sf::Color::Black);

		startBtn.SetText("Start");

		buttons.push_back(std::make_shared<GUI::Button>(startBtn));

		std::vector<std::shared_ptr<Object>> mouseList = CreateObjectFromFile("data/obj/Mouse_Data.xml", sf::Vector2f(0, 0));
		objects.Add(mouseList);*/

	};
	void OnDestroy() override
	{

	};

	void OnActivate() override
	{
		sf::View view = window.GetView();
		view.reset(sf::FloatRect(0,0, sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
		window.SetView(view);
	};

	void Update(float deltaTime) override
	{
		Scene::Update(deltaTime);

		if (input.IsKeyPressed(Input::Key::Esc))
		{
			this->window.Close();
		}
		
		ShowMainMenu(&p_open);

		/*for (auto& b : buttons)
		{
			b->Update(sf::Vector2f(sf::Mouse::getPosition(window.GetRenderWindow())));

			if (b->GetName() == "StartBtn")
			{
				if (b->isPressed())
				{
					switchScene = true;
				}
			}
		}*/

	};
	
	void Draw(Window& window) override
	{
		//window.Draw(this->background);

		Scene::Draw(window);

		/*for (auto& b : buttons)
		{
			b->Draw(window);
		}*/
	};

	void ShowMainMenu(bool* p_open)
	{
		window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

		static int startClicked = 0, loadClicked = 0, exitClicked =0;

		ImGui::SetNextWindowPos(ImVec2((window.GetRenderWindow().getSize().x - ImGui::GetWindowWidth()) * 0.5f, (window.GetRenderWindow().getSize().y - ImGui::GetWindowHeight()) * 0.75f));

		if (ImGui::Begin("MainMenu", p_open, window_flags))
		{
			if (CreateImageButton("btnStart", "Start", *buttonTexture)) startClicked++;
			if (CreateImageButton("btnLoad", "Continue", *buttonTexture)) loadClicked++;
			if (CreateImageButton("btnExit", "Exit", *buttonTexture)) exitClicked++;
			
			if (startClicked & 1)
			{
				switchScene = true;
				startClicked = 0;
			}
			if (loadClicked & 1)
			{
				//Load save data
				loadClicked = 0;
			}
			if (exitClicked & 1)
			{
				window.Close();
				exitClicked = 0;
			}

		};

		ImGui::End();
	};

	
private:
	/*sf::RectangleShape background;
	GUI::Grid componentGrid;*/
	
	/*std::vector<std::shared_ptr<GUI::Button>> buttons;*/
	
	//Ui Stuff

	sf::Texture* buttonTexture;

	ImGuiWindowFlags window_flags;  //Specifies info about the window
	bool p_open; //If window is open
};

#endif //SceneMainMenu_h