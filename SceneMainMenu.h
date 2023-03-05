#pragma once
#ifndef SceneMainMenu_h
#define SceneMainMenu_h

#include "Input.h"
#include "GUI.h"
#include "Scene.h"

class SceneMainMenu : public Scene
{
public:
	SceneMainMenu(WorkingDirectory& workingDir, ResourceAllocator<sf::Texture>& textureAllocator, Window& window, ResourceAllocator<sf::Font>& fontAllocator) : 
		Scene(workingDir, textureAllocator, window, fontAllocator){};
	
	
	void OnCreate() override
	{
		Scene::OnCreate();

		componentGrid.Load();

		this->background.setSize(sf::Vector2f(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
		this->background.setFillColor(sf::Color::White);

		std::string btnName = "StartBtn";
		const int fontID = fontAllocator.Add(workingDir.Get() + "Assets/Fonts/joystix monospace.ttf");
		std::shared_ptr<sf::Font> font = fontAllocator.Get(fontID);

		GUI::Button startBtn(btnName, 100, 100, 100, 100, font, 16,
			sf::Color::Black, sf::Color::Black, sf::Color::Black, sf::Color::Blue, sf::Color::Cyan, sf::Color::Black);

		startBtn.SetText("Start");

		buttons.push_back(std::make_shared<GUI::Button>(startBtn));

		std::vector<std::shared_ptr<Object>> mouseList = CreateObjectFromFile("data/obj/Mouse_Data.xml", sf::Vector2f(0, 0));
		objects.Add(mouseList);

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
		
		for (auto& b : buttons)
		{
			b->Update(sf::Vector2f(sf::Mouse::getPosition(window.GetRenderWindow())));

			if (b->GetName() == "StartBtn")
			{
				if (b->isPressed())
				{
					switchScene = true;
				}
			}
		}

	};
	
	void Draw(Window& window) override
	{
		window.Draw(this->background);

		Scene::Draw(window);

		for (auto& b : buttons)
		{
			b->Draw(window);
		}
	};

	
private:
	sf::RectangleShape background;
	GUI::Grid componentGrid;
	
	std::vector<std::shared_ptr<GUI::Button>> buttons;

	
};

#endif //SceneMainMenu_h