#pragma once

#ifndef UI_Button_h
#define UI_Button_h

#include "C_Drawable.h"
#include "BaseUserInterface.h"

class UI_Button : public BaseUserInterface
{
public:

	UI_Button(SharedContext* context) : BaseUserInterface(context), r(sf::RectangleShape(sf::Vector2f(0,0))) {}; //funcPtr is the function pointer to register for when the button is clicked

	bool isClicked(float mouseX, float mouseY)
	{
		//Does not check if mouse is actually clicked, just if the location of the mouse is inside the button bounds.
		return mouseX > GetPosition().x && mouseX < GetPosition().x + GetScaledSize().x &&
			mouseY > GetPosition().y && mouseY < GetPosition().y + GetScaledSize().y;
	};

	void UpdateUI()
	{
		if (GetSomethingChanged())
		{
			r.setSize(GetScaledSize());
			r.setPosition(GetPosition());
			Reload();
		}
	};

	void Draw(Window& window) override
	{
		window.Draw(r);
	};

private:

	sf::RectangleShape r;
};



#endif // UI_Button_h