#pragma once

#ifndef C_Dialogue_h
#define C_Dialogue_h

#include <unordered_map>
#include "Component.h"
#include "Quest.h"
#include "Button.h"
#include "C_Drawable.h"
#include "Object.h"

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
	C_Dialogue(Object* owner) : Component(owner), doneTalking(false), curNodeID(0), nextNodeID(0) {};
	~C_Dialogue() = default;

	void Start() override
	{
		const int fontID = owner->context->fontAllocator->Add(owner->context->workingDir->Get() + "Assets/Fonts/joystix monospace.ttf");
		font = owner->context->fontAllocator->Get(fontID);
		dialogueText.setFont(*font);
		CreateBackground();
	};

	void Draw(Window& window) override
	{
		window.Draw(background);
		window.Draw(dialogueText);

		for (auto& it : optionButtons)
		{
			it.Draw(window);
		}
	};

	bool ContinueToDraw() const override
	{
		return !doneTalking;
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

	void GoToNext(int nextNodeID, StatusType returnCode)
	{
		curNodeID += 1; 
		std::shared_ptr <DialogueNode> node = GetDialogueNode();

		if (nextNodeID == -1) //If there are no options and not looping
		{
			if (!node)
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
		}

		ParseDialogueNode(node);
	};

	void ParseDialogueNode(std::shared_ptr<DialogueNode> curNode)
	{
		dialogueText.setString(curNode->text);

		if (curNode->options.size() > 0)
		{
			for (auto& it : curNode->options)
			{
				optionButtons.push_back(CreateButton(it));
			}
		}
	};

	std::unordered_map<int, std::unordered_map<StatusType, std::vector<std::shared_ptr<DialogueNode>>>> allDialogue;

	std::shared_ptr<Quest> GetCurQuest() { return curQuest; };
	void SetCurQuest(std::shared_ptr<Quest> q) { curQuest = q; };

	
private:
	
	Button<C_Dialogue, void, int, StatusType> CreateButton(std::shared_ptr<OptionNode> it)
	{
		Button<C_Dialogue, void, int, StatusType> btn(*this, &C_Dialogue::GoToNext);
		btn(it->nextNodeID, it->returnCode);
		btn.SetText(it->text, font);
		btn.SetSize(sf::Vector2f(100, 50));
		btn.SetPosition(sf::Vector2f(500, 500));
		btn.SetBackgroundColour(sf::Color::Black);
		btn.SetFontColour(sf::Color::White);

		return btn;
	};

	void CreateBackground()
	{
		sf::Vector2f v_center = owner->context->window->GetView().getCenter();
		sf::Vector2f v_size = owner->context->window->GetView().getSize();
		bgPadding = {10.0, 10.0, 10.0, 20.0};

		float bg_sizeX = v_size.x - bgPadding.left - bgPadding.right;
		float bg_sizeY = (v_size.y / 3) - bgPadding.top - bgPadding.bottom;

		float bg_positionX = (v_center.x - v_size.x / 2) + bgPadding.left;
		float bg_positionY = (v_center.y + v_size.y / 6) + bgPadding.top;

		background.setSize(sf::Vector2f(bg_sizeX, bg_sizeY));
		background.setPosition(sf::Vector2f(bg_positionX, bg_positionY));
		background.setFillColor(sf::Color::Cyan);
	};


	std::shared_ptr<Quest> curQuest;
	int curNodeID, nextNodeID;
	std::shared_ptr<DialogueNode> curNode;

	sf::Text dialogueText;
	std::vector<Button<C_Dialogue, void, int, StatusType>> optionButtons;
	std::shared_ptr<sf::Font> font;
	sf::RectangleShape background;

	UIPadding bgPadding;
	

	bool doneTalking;
};

#endif // C_Dialogue_h
