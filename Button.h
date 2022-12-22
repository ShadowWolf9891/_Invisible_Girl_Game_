#pragma once

#ifndef Button_h
#define Button_h

#include "C_Drawable.h"

/*
	Template class for button can be registered with
	
	Button<[Target Class], [Return Value], [Arg1], [Arg2]...> buttonName([classInstance], &[MyClass]::[MyMethod]);

	"this" can be used as target if needed.
*/
template < class C, class Tr, class... Args >
class Button : public C_Drawable
{
public:

	typedef Tr(C::*EventMethod)(Args...);

	Button(C& target, EventMethod method) : m_target(target), m_method(method), isVisible(false){}; //funcPtr is the function pointer to register for when the button is clicked

	~Button() {};

	bool operator() (Args&... args) const
	{
		(m_target.*(m_method))(args...);

		return true;
	};
	
	void Draw(Window& window) override
	{
		window.Draw(background);
		window.Draw(text);
	};

	bool ContinueToDraw() const override
	{
		return isVisible;
	}

	void SetPosition(sf::Vector2f pos) 
	{
		const sf::FloatRect& backBounds = background.getLocalBounds();
		const sf::Vector2f centeredPosition = sf::Vector2f(pos.x - (backBounds.width * 0.5f), pos.y - (backBounds.height * 0.5f));

		background.setPosition(centeredPosition);
		text.setPosition(centeredPosition);
	};
	sf::Vector2f GetPosition() { return background.getPosition(); };

	sf::Vector2f GetSize() { return background.getSize(); };

	void SetText(const std::string& text, std::shared_ptr<sf::Font> font) 
	{
		this->text.setString(text); 
		this->text.setFont(*font);
		this->text.scale(0.5, 0.5);
		background.setSize(sf::Vector2f(this->text.getGlobalBounds().width * 2, this->text.getGlobalBounds().height * 2));
	};

	void SetFontColour(const sf::Color& colour) { text.setFillColor(colour); text.setOutlineColor(sf::Color::Black); };
	void SetBackgroundColour(const sf::Color& colour) { background.setFillColor(colour); };

	void SetVisible(bool visibility) { isVisible = visibility; };
	bool IsVisible() { return isVisible; };

	sf::RectangleShape GetBackground() { return background; };

private:

	sf::Text text;
	EventMethod m_method;
	C& m_target;
	sf::RectangleShape background;
	bool isVisible;
};



#endif // Button_h