#pragma once

#ifndef C_Dialogue_h
#define C_Dialogue_h

#include <unordered_map>
#include "Component.h"
#include "Quest.h"
#include "UI_Button.h"
#include "C_Drawable.h"
#include "Input.h"
#include "UI_TextBox.h"
#include "BaseUserInterface.h"
#include "UI_Lists.h"

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

//struct UIPadding
//{
//	float left = 0.0f, right = 0.0f, top = 0.0f, bottom = 0.0f;
//
//};

class C_Dialogue : public Component, public C_Drawable
{
public:
	C_Dialogue(Object* owner) : Component(owner), doneTalking(false), isVisible(false), curNodeID(0), nextNodeID(0), delay(0), mouseBox(0,0,10,10) {};
	~C_Dialogue() = default;

	void Start() override
	{
		const int fontID = owner->context->fontAllocator->Add(owner->context->workingDir->Get() + "Assets/Fonts/joystix monospace.ttf");
		font = owner->context->fontAllocator->Get(fontID);
		
		dialogueText.setFont(*font);
		dialogueText.setScale(0.5, 0.5);

		curNode = GetDialogueNode();
	};

	void LoadUIFormat(std::unordered_map<std::string, UIData> formatData)
	{
		for (auto& o : formatData)
		{
			if (o.second.uiType == "HorizontalList") CreateUIObject<UI_HorizontalList>(o.first, o.second);
			else if (o.second.uiType == "VerticleList") CreateUIObject<UI_VerticleList>(o.first, o.second);
			else if (o.second.uiType == "TextBox") CreateUIObject<UI_TextBox>(o.first, o.second);
			else if (o.second.uiType == "Button") CreateUIObject<UI_Button>(o.first, o.second);
			else std::cout << "Unknown UI Object type:" << o.second.uiType << " for object " << o.first << std::endl;
		}

	}


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
		
		for (auto& dObj : dialogueUI)
		{
			//dObj->UpdateUI();
		}

	};


	void Draw(Window& window) override
	{
		if (!IsVisible()) return;
		
		for (auto& drawable : drawables)
		{
			//drawable->Draw(window);
		}


		/*window.Draw(background);
		window.Draw(dialogueText);

		for (auto& it : optionButtons)
		{
			for (auto& it2 : it.second.GetChildren())
			{
				if (std::dynamic_pointer_cast<UI_TextBox>(it2))
				{
					it2->UI_TextBox::Draw()
				}
			}
		}*/
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

	template <typename T>
	void CreateUIObject(std::string name, UIData objData)
	{
		static_assert(std::is_base_of<BaseUserInterface, T>::value, "T must derive from BaseUserInterface.");

		//Use constexpr to check if a specific method exists for the typename. If it doesn't it returns false.
		constexpr bool has_ChildSpacing = requires(T & t) { t.GetChildSpacing();};
		constexpr bool has_TextWrapping = requires(T & t) {t.GetTextWrapping();};
		constexpr bool has_TextAlign = requires(T & t) { t.GetAlignment(); };
		
		//Other optional properties go here:

		int scaleX = 1, scaleY = 1;
		std::shared_ptr<T> uiObj = std::make_shared<T>(owner->context); // Create ui Object

		 //All objects must have a name
		uiObj->uiType = objData.uiType;

		for (auto& p : objData.properties) //Loop through all properties
		{
			if(p.first == "Name") uiObj->SetName(p.second);
			else if (p.first == "Anchor") uiObj->SetAnchor(strToAnchor.at(p.second));
			else if (p.first == "FillType") uiObj->SetFillType(strToFillType.at(p.second));
			else if (p.first == "ScaleX") scaleX = atof(p.second.c_str());
			else if (p.first == "ScaleY") scaleY = atof(p.second.c_str());
			else if (p.first == "ChildSpacing")
			{
				if constexpr(has_ChildSpacing) uiObj->SetChildSpacing(atof(p.second.c_str()));
			}
			else if (p.first == "TextWrapping") 
			{
				if constexpr(has_TextWrapping) uiObj->SetTextWrapping(atoi(p.second.c_str()));
			}
			else if (p.first == "TextAlign")
			{
				if constexpr (has_TextAlign) uiObj->SetAlignment(atoi(p.second.c_str()));
			}

			//Any future properties go here. Don't forget to add a constexp above if it is not shared by all objects.

			else std::cout << "Unknown UI Object property: " << p.first << std::endl;
		}

		uiObj->SetScale(scaleX, scaleY); //Set scale from above values

		
			if (dialogueUI.find(objData.parentName) != dialogueUI.end())
			{
				if (objData.parentName != "Root")
				{
					if (dialogueUI.at(objData.parentName)->uiType == "HorizontalList")
					{
						uiObj->SetParent<UI_HorizontalList>(std::dynamic_pointer_cast<UI_HorizontalList>(dialogueUI.at(objData.parentName)));
					}
				}
			}
			else
			{
				std::cout << "Something went wrong assigning UI parent and childs" << std::endl;
			}
		
		
		dialogueUI.insert(std::make_pair(uiObj->GetName(), uiObj));

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
			/*for (auto& it : curNode->options)
			{
				optionButtons.insert(std::make_pair(it, CreateButton(it, optionNum)));
				optionNum++;
			}*/
		}
	};

	std::unordered_map<int, std::unordered_map<StatusType, std::vector<std::shared_ptr<DialogueNode>>>> allDialogue;

	std::shared_ptr<Quest> GetCurQuest() { return curQuest; };
	void SetCurQuest(std::shared_ptr<Quest> q) { curQuest = q; };

	void SetVisible(bool visibility) { isVisible = visibility; };
	bool IsVisible() { return isVisible; };

private:
	
	void CreateBackground()
	{
		/*sf::Vector2f v_center = owner->context->window->GetView().getCenter();
		sf::Vector2f v_size = owner->context->window->GetView().getSize();
		bgPadding = {20.0, 20.0, 20.0, 20.0};

		float bg_sizeX = v_size.x - bgPadding.left - bgPadding.right;
		float bg_sizeY = (v_size.y / 3) - bgPadding.top - bgPadding.bottom;

		float bg_positionX = (v_center.x - v_size.x / 2) + bgPadding.left;
		float bg_positionY = (v_center.y + v_size.y / 6) + bgPadding.top;

		background.setSize(sf::Vector2f(bg_sizeX, bg_sizeY));
		background.setPosition(sf::Vector2f(bg_positionX, bg_positionY));
		background.setFillColor(sf::Color::Cyan);*/
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
				float mousePosX = owner->context->window->GetViewSpace().left + owner->context->input->GetMousePos().x;
				float mousePosY = owner->context->window->GetViewSpace().top + owner->context->input->GetMousePos().y;
				sf::Vector2f mousePos = sf::Vector2f(mousePosX,mousePosY);
				
				mouseBox.left = mousePos.x;
				mouseBox.top = mousePos.y;

				//for (auto& btn : optionButtons)
				//{
				//	/*if (btn.second.GetCollidable().intersects(mouseBox))
				//	{
				//		btn.second(btn.first->nextNodeID, btn.first->returnCode);
				//	}*/
				//}
			}
		}
			
		

	};

	std::shared_ptr<Quest> curQuest;
	int curNodeID, nextNodeID;
	std::shared_ptr<DialogueNode> curNode;

	std::unordered_map<std::string, std::shared_ptr<BaseUserInterface>> dialogueUI;
	std::unordered_map<std::string, std::shared_ptr<C_Drawable>> drawables;

	sf::Text dialogueText;
	//std::unordered_map< std::shared_ptr<OptionNode>, UI_Button> optionButtons;
	std::shared_ptr<sf::Font> font;
	sf::RectangleShape background;

	sf::FloatRect mouseBox;

	float delay;
	
	bool isVisible;
	bool doneTalking;
};

#endif // C_Dialogue_h