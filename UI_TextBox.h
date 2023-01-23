#pragma once

#ifndef UI_TextBox_h
#define UI_TextBox_h

#include "C_Drawable.h"
#include "BaseUserInterface.h"

enum class TextWrapping
{
	NONE = 0,			//No text wrapping. If text is larger than the textbox, it will not be displayed.
	AUTO = 1,			//Automatically handle text wrapping. Different for each box format. Will always display all text.
	FILL = 2			//Fill the box with the text 
};

enum class TextAdjust
{
	DEFAULT = 0,		//No change. Use this for dynamic BoxFormat.
	LEFT = 1,			//Force text of each line to start at the left of the textbox.
	CENTER = 2,			//Force text of each line to start at the center of the textbox.
	RIGHT = 3			//Force text of each line to start at the right of the textbox.
};

class UI_TextBox : public BaseUserInterface
{
public:
	UI_TextBox(SharedContext* context) : BaseUserInterface(context), wrapping(TextWrapping::NONE), adjustment(TextAdjust::DEFAULT) {};

	void LoadUI() override
	{
		const int fontID = context->fontAllocator->Add(context->workingDir->Get() + "Assets/Fonts/joystix monospace.ttf");
		font = context->fontAllocator->Get(fontID);

		drawnText.setFont(*font);
	};

	void UpdateUI() override
	{
		if (GetSomethingChanged())
		{
			Reload();
			FormatText();
			SetSomethingChanged(false);
		}
	};

	void Draw(Window& window) override
	{
		window.Draw(drawnText);
	};

	bool ContinueToDraw() const override
	{
		if (GetText().empty()) {
			return false;
		}
		return true;
	};

	void SetCharSize(unsigned int charSize) { drawnText.setCharacterSize(charSize); SetSomethingChanged(true);};
	unsigned int GetCharSize() const { return drawnText.getCharacterSize(); };
	
	void SetText(std::string text) { this->text = text; SetSomethingChanged(true);};
	std::string GetText() const { return text; };

	void SetTextWrapping(TextWrapping tw) { wrapping = tw; SetSomethingChanged(true);};
	void SetTextWrapping(int tw) { wrapping = TextWrapping(tw); SetSomethingChanged(true); };
	TextWrapping GetTextWrapping() const { return wrapping; };

	void SetAlignment(TextAdjust alignment) { adjustment = alignment; SetSomethingChanged(true);};
	void SetAlignment(int alignment) { adjustment = TextAdjust(alignment); SetSomethingChanged(true); };
	TextAdjust GetAlignment() const { return adjustment; };

private:

	void FormatText() 
	{
		float pWidth, pHeight;

		if (GetParent())
		{
			pWidth = GetParent()->GetScaledSize().x;
			pHeight = GetParent()->GetScaledSize().y;
		}

		int numLines = (text.size() * GetCharSize() / pWidth) + 1;
		int charPerLine = text.size() / numLines;

		switch (wrapping)
		{
		case(TextWrapping::NONE):
			drawnText.setString(text);
		case(TextWrapping::AUTO):
			drawnText.setString(WrapText(text, numLines, charPerLine));
		case(TextWrapping::FILL):
			SetCharSize(pWidth / text.size() / numLines);
			drawnText.setString(WrapText(text, numLines, charPerLine));
		default:
			std::cout << "Invalid UI text wrapping: " << text << std::endl;
		}


		//TODO: Adjustment by seperating each line into invividual strings with their own transforms and drawing the vector of them
		switch (adjustment)
		{
		case(TextAdjust::DEFAULT):
			
		case(TextAdjust::LEFT):
			SetAnchor(Anchor::TOPLEFT);
		case(TextAdjust::CENTER):
			SetAnchor(Anchor::TOPMID);
		case(TextAdjust::RIGHT):
			SetAnchor(Anchor::TOPRIGHT);
		default:
			SetAnchor(Anchor::TOPLEFT);
		}
	};

	std::string WrapText(std::string originalString, int numLines, int charPerLine)
	{
		std::size_t foundSpace = 0;
		std::size_t previousSpace = 0;
		std::string newString = originalString;

		for (int i = 1; i < numLines; i++)
		{
			while (foundSpace < i * charPerLine)
			{
				previousSpace = foundSpace;
				foundSpace = originalString.find(" ", foundSpace);
			}

			newString.insert(previousSpace, "\n");
		}

		return newString;
	}


	std::string text;
	sf::Text drawnText;
	std::shared_ptr<sf::Font> font;
	sf::RectangleShape background;
	sf::Vector2f boxSize;

	TextWrapping wrapping;
	TextAdjust adjustment;
	
};


#endif // UI_TextBox_h