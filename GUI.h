#pragma once

#ifndef GUI_h
#define GUI_h

#include <iostream>

enum button_states{BTN_IDLE = 0, BTN_HOVER = 1, BTN_ACTIVE = 2};
enum list_orientation{HORIZONTAL = 0, VERTICAL = 1, GRID = 2};



namespace GUI
{
	class Material : public sf::Drawable
	{
	public:
		//Constructor
		Material(): size(sf::Vector3f(1,1,1)), position(sf::Vector3f(0,0,0)){};
		~Material() = default;
		
		//Get and Set

		void SetXPos(float x) { position.x = x; };
		void SetYPos(float y) { position.y = y; };
		void SetLayer(float z) { position.z = z; };

		void SetWidth(float width) { size.x = width; };
		void SetLength(float length) { size.y = length; };
		void SetSize(sf::Vector2f size) { this->size.x = size.x, this->size.y = size.y; };

		sf::Vector2f GetPosition() { return sf::Vector2f(position.x, position.y); };
		float GetLayer() { return position.z; };
		sf::Vector2f GetSize() { return sf::Vector2f(size.x, size.y); };
		sf::FloatRect GetLocalBounds() { return sf::FloatRect(0, 0, size.x, size.y); };
		sf::FloatRect GetGlobalBounds() { return sf::FloatRect(position.x, position.y, size.x, size.y); };

	private:
		sf::Vector3f position;
		sf::Vector3f size;
	};

	class Cursor : public sf::Drawable
	{
	public:

		/*Cursor(Scene& curScene): curScene(curScene)
		{
			
		
		};*/
		//~Cursor() = default;

		void HandleMouseEvents(sf::Event event)
		{

		};

		void Draw(Window& window)
		{
			
		};

	private:
		//Scene& curScene;
		//Object cursor;

	};


	class Grid
	{
	public:
		Grid(){};
		
		~Grid() = default;

		void Load()
		{
			windowSize.x = sf::VideoMode::getDesktopMode().width;
			windowSize.y = sf::VideoMode::getDesktopMode().height;
			dp = sf::VideoMode::getDesktopMode().bitsPerPixel;




			componentGrid = SetupGrid(4);
		};
		
	private:

		std::vector<std::vector<sf::Vector2i>> SetupGrid(const int gridSpacing)
		{
			std::vector<std::vector<sf::Vector2i>> tempGrid;

			for (int i = 0; i < windowSize.y; i += gridSpacing)
			{
				std::vector<sf::Vector2i> row;

				for (int j = 0; j < windowSize.x; j += gridSpacing)
				{
					row.push_back(sf::Vector2i(j, i));
				}

				tempGrid.push_back(row);
			}

			return tempGrid;
		};

		//BaseLineGrid
		float Align(float value, const float GRID_SQUARE_SIZE)
		{
			return std::floor(value / GRID_SQUARE_SIZE) * GRID_SQUARE_SIZE;
		}

		std::vector<std::vector<sf::Vector2i>> componentGrid;
		
		sf::Vector2f windowSize;

		float dp;
		
		

	};


	class Button
	{
	public:
		Button(std::string name, float x, float y, float width, float height,
			std::shared_ptr<sf::Font> font, unsigned character_size,
			const sf::Color text_Idle_Color, const sf::Color text_Hover_Color, const sf::Color text_Active_Color,
			const sf::Color idle_Color, const sf::Color hover_Color, const sf::Color active_Color)
		{
			this->name = name;
			this->buttonState = BTN_IDLE;
			
			this->textIdleColor = text_Idle_Color;
			this->textHoverColor = text_Hover_Color;
			this->textActiveColor = text_Active_Color;

			this->idleColor = idle_Color;
			this->hoverColor = hover_Color;
			this->activeColor = active_Color;

			this->shape.setPosition(sf::Vector2f(x, y));
			this->shape.setSize(sf::Vector2f(width, height));
			this->shape.setFillColor(idle_Color);

			this->font = font;
			this->text.setFont((*this->font));
			this->text.setFillColor(text_Idle_Color);
			this->text.setCharacterSize(character_size);
			
			
			
		};

		~Button() = default;

		const bool isPressed() const
		{
			if (this->buttonState == BTN_ACTIVE) return true;
			return false;
		};

		void Update(const sf::Vector2f& mousePos)
		{
			//Set Idle by default
			this->buttonState = BTN_IDLE;

			//Hover

			if (shape.getGlobalBounds().contains(mousePos))
			{
				this->buttonState = BTN_HOVER;

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					this->buttonState = BTN_ACTIVE;
				}
			}

			switch (this->buttonState)
			{
			case BTN_IDLE:
				this->shape.setFillColor(this->idleColor);
				this->text.setFillColor(this->textIdleColor);
				break;
			case BTN_HOVER:
				this->shape.setFillColor(this->hoverColor);
				this->text.setFillColor(this->textHoverColor);
				break;
			case BTN_ACTIVE:
				this->shape.setFillColor(this->activeColor);
				this->text.setFillColor(this->textActiveColor);
				break;
			default:
				this->shape.setFillColor(sf::Color::Red);
				this->text.setFillColor(sf::Color::Blue);
				break;
			}
		};

		void Draw(Window& window)
		{
			window.Draw(this->shape);
			window.Draw(this->text);
		};

		void SetText(std::string text) { 
			this->text.setString(text);
			this->text.setPosition(
				shape.getGlobalBounds().left + (this->shape.getGlobalBounds().width / 2.f) - (this->text.getGlobalBounds().width / 2),
				shape.getGlobalBounds().top + (this->shape.getGlobalBounds().height / 2.f) - (this->text.getGlobalBounds().height / 2));
		};

		std::string GetName() { return name; };

	private:
		std::string name;
		short unsigned buttonState;
		sf::RectangleShape shape;
		std::shared_ptr<sf::Font> font;
		sf::Text text;

		sf::Color textIdleColor;
		sf::Color textHoverColor;
		sf::Color textActiveColor;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;
	};

	class TextBox
	{
	public:
		TextBox(float x, float y, float width, float height,
			sf::Font* font, unsigned character_size,
			sf::Color text_Color, sf::Color shape_Color) 
		{
			this->shape.setPosition(sf::Vector2f(x, y));
			this->shape.setSize(sf::Vector2f(width, height));
			this->shape.setFillColor(shape_Color);
			this->font = font;
			this->text.setFont((*this->font));
			this->text.setFillColor(text_Color);
			this->text.setCharacterSize(character_size);
			this->text.setPosition(
				this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
				this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 2.f
			);
		
			this->textColor = text_Color;
			this->shapeColor = shape_Color;
		};
		
		~TextBox() = default;

		void Draw(Window& window)
		{
			window.Draw(this->shape);
			window.Draw(this->text);
		};
		
		void SetText(std::string text) {this->text.setString(text);};

	private:

		sf::RectangleShape shape;
		sf::Font* font;
		sf::Text text;

		sf::Color textColor;
		sf::Color shapeColor;

	};

	class DropDownBox
	{
	private:
		void Update()
		{

		};
		void Draw(Window& window)
		{

		};
	};

};

#endif // GUI_h