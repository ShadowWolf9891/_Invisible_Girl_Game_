#pragma once

#ifndef C_Dialogue_h
#define C_Dialogue_h

#include <unordered_map>
#include "Component.h"
#include "Quest.h"
#include "Button.h"
#include "C_Drawable.h"
#include "Object.h"
#include "Input.h"

struct OptionNode
{
	int nextNodeID;
	std::string text;
	StatusType returnCode;
};

struct DialogueNode
{
	int ID;
	std::string text;
	std::vector<std::shared_ptr<OptionNode>> options;
};

struct UIPadding
{
	float left = 0.0f, right = 0.0f, top = 0.0f, bottom = 0.0f;

};

class C_Dialogue : public Component, public C_Drawable
{
public:
	C_Dialogue(Object* owner) : Component(owner), doneTalking(false), isVisible(false), curNodeID(0), nextNodeID(0), delay(0) {};
	~C_Dialogue() = default;

	void Start() override
	{
		const int fontID = owner->context->fontAllocator->Add(owner->context->workingDir->Get() + "Assets/Fonts/joystix monospace.ttf");
		font = owner->context->fontAllocator->Get(fontID);
		
		dialogueText.setFont(*font);
		dialogueText.setScale(0.5, 0.5);

		curNode = GetDialogueNode();

	};

	void Update(float deltaTime) override
	{
		if (!IsVisible()) { return; }

		if (delay >= 0.1)
		{
			delay = 0;
			CheckInput();
		}
		else
		{
			delay += deltaTime;
		}
		


	};


	void Draw(Window& window) override
	{
		if (!IsVisible()) return;
	
		window.Draw(background);
		window.Draw(dialogueText);

		for (auto& it : optionButtons)
		{
			it.second.Draw(window);
		}
	};

	bool ContinueToDraw() const override
	{
		return !doneTalking;
	};

	void CreateUI() 
	{
		CreateBackground();
		ParseDialogueNode(GetDialogueNode());
		isVisible = true;
	};

	std::shared_ptr <DialogueNode> GetDialogueNode()
	{
		std::shared_ptr <DialogueNode> node = allDialogue.at(curQuest->GetID()).at(curQuest->GetStatus()).at(curNodeID);

		if (node) 
		{
			return node;
		}
		
		return nullptr;
	};

	void GoToNext(int nextNodeID = -1, StatusType returnCode = StatusType::UNAVAILABLE)
	{
		if (nextNodeID == -1) //If there are no options and not looping
		{
			curNodeID += 1; 
			curNode = GetDialogueNode();

			if (!curNode)
			{
				doneTalking = true;
				curNodeID = 0;
				curQuest->SetStatus(returnCode);
				return;
			}
		}
		else
		{
			curNodeID = nextNodeID;
			curNode = GetDialogueNode();
		}

		ParseDialogueNode(curNode);
	};

	void ParseDialogueNode(std::shared_ptr<DialogueNode> curNode)
	{
		dialogueText.setString(curNode->text);

		float xPos = background.getGlobalBounds().left + background.getGlobalBounds().width / 2 - dialogueText.getGlobalBounds().width / 2;
		float yPos = background.getPosition().y;
		dialogueText.setPosition(sf::Vector2f(xPos,yPos));
		

		if (curNode->options.size() > 0)
		{
			int optionNum = 0;
			for (auto& it : curNode->options)
			{
				optionButtons.insert(std::make_pair(it, CreateButton(it, optionNum)));
				optionNum++;
			}
		}
	};

	std::unordered_map<int, std::unordered_map<StatusType, std::vector<std::shared_ptr<DialogueNode>>>> allDialogue;

	std::shared_ptr<Quest> GetCurQuest() { return curQuest; };
	void SetCurQuest(std::shared_ptr<Quest> q) { curQuest = q; };

	void SetVisible(bool visibility) { isVisible = visibility; };
	bool IsVisible() { return isVisible; };

private:
	
	Button<C_Dialogue, void, int, StatusType> CreateButton(std::shared_ptr<OptionNode> it, int optionNum)
	{
		Button<C_Dialogue, void, int, StatusType> btn(*this, &C_Dialogue::GoToNext);
		btn.SetText(it->text, font);
		btn.SetPosition(sf::Vector2f(background.getPosition().x + background.getSize().x / 2, (background.getPosition().y + background.getSize().y / 4) + (20 * optionNum)));
		btn.SetBackgroundColour(sf::Color::Black);
		btn.SetFontColour(sf::Color::White);
		btn.SetVisible(true);

		return btn;
	};

	void CreateBackground()
	{
		sf::Vector2f v_center = owner->context->window->GetView().getCenter();
		sf::Vector2f v_size = owner->context->window->GetView().getSize();
		bgPadding = {20.0, 20.0, 20.0, 20.0};

		float bg_sizeX = v_size.x - bgPadding.left - bgPadding.right;
		float bg_sizeY = (v_size.y / 3) - bgPadding.top - bgPadding.bottom;

		float bg_positionX = (v_center.x - v_size.x / 2) + bgPadding.left;
		float bg_positionY = (v_center.y + v_size.y / 6) + bgPadding.top;

		background.setSize(sf::Vector2f(bg_sizeX, bg_sizeY));
		background.setPosition(sf::Vector2f(bg_positionX, bg_positionY));
		background.setFillColor(sf::Color::Cyan);
	};

	void CheckInput()
	{
		if (!curNode) { return; }

		/*if (owner->context->input->IsKeyPressed(Input::Key::E))
		{
			if (curNode->options.empty()) 
			{
				GoToNext(-1, curQuest->GetStatus());
				return;
			}
		}*/
		if (owner->context->input->IsMousePressed(Input::Mouse::Left))
		{
			if (curNode->options.empty())
			{
				GoToNext(-1, curQuest->GetStatus());
				return;
			}
			else 
			{
				sf::Vector2f mousePos = (sf::Vector2f)(owner->context->input->GetMousePos());
				for (auto& btn : optionButtons)
				{
					if (btn.second.GetBackground().getGlobalBounds().contains(mousePos))
					{
						btn.second(btn.first->nextNodeID, btn.first->returnCode);
					}
				}
			}
		}
			
		

	};

	std::shared_ptr<Quest> curQuest;
	int curNodeID, nextNodeID;
	std::shared_ptr<DialogueNode> curNode;

	sf::Text dialogueText;
	std::unordered_map< std::shared_ptr<OptionNode>, Button<C_Dialogue, void, int, StatusType>> optionButtons;
	std::shared_ptr<sf::Font> font;
	sf::RectangleShape background;

	UIPadding bgPadding;

	float delay;
	
	bool isVisible;
	bool doneTalking;
};

#endif // C_Dialogue_h
