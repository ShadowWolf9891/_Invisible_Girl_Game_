#pragma once

#ifndef BaseUserInterface_h
#define BaseUserInterface_h

#include "Component.h"
#include "Object.h"
#include "C_Drawable.h"

enum class Anchor
{
	TOPLEFT = 0,
	TOPMID = 1,
	TOPRIGHT = 2,
	MIDLEFT = 3,
	CENTER = 4,
	MIDRIGHT = 5,
	BOTLEFT = 6,
	BOTMID = 7,
	BOTRIGHT = 8
};

enum class FillType
{
	AUTO = 0,				//Fit to contents
	SET = 1,				//Size is ratio of parent or window. If the window is 1000x1000 and scale is 0.5, 0.5, the resulting child will have a size 500x500 
	FILLHORIZONTAL = 2,		//Fill the container horizontally 
	FILLVERTICAL = 3,		//Fill the container vertically
	FILL = 4				//Fill the container
};

struct Padding
{
	float left = 0.0f, right = 0.0f, top = 0.0f, bottom = 0.0f;
};

class BaseUserInterface : public C_Drawable
{
public:
	BaseUserInterface(SharedContext* context) : C_Drawable(), context(context), anchor(Anchor::TOPLEFT), fillType(FillType::AUTO), scale(1,1), position(0,0), offset(0,0), size(0,0), padding(Padding(0,0,0,0)), somethingChanged(false), visible(false), r(sf::RectangleShape(sf::Vector2f(0, 0))) {};

	virtual ~BaseUserInterface() = default;

	virtual void LoadUI()
	{
		SetDrawLayer(DrawLayer::UI);
		SetColour(sf::Color::Black);
	};
	virtual void UpdateUI()
	{
		if (GetSomethingChanged())
		{
			Reload();
		}
	};
	 

	virtual void Reload() 
	{
		SetupSize();
		SetPositon();

		//if (!children.empty())	//If this object has children, tell them to reload as well
		//{
		//	for (auto& child : children)
		//	{
		//		child->SetSomethingChanged(true);
		//	}
		//}

		r.setSize(GetScaledSize());
		r.setPosition(GetPosition());
		SetSomethingChanged(false);

		std::cout << "Reloaded: " << this->GetName() << " Position: x= "<< GetPosition().x << ", y= " << GetPosition().y << " Size: width= " << GetScaledSize().x << ", height= " << GetScaledSize().y << std::endl;
	};

	virtual void SetPadding(float left, float right, float top, float bottom) { padding.left = left; padding.right = right; padding.top = top; padding.bottom = bottom; SetSomethingChanged(true);};
	virtual void SetPadding(Padding p) { padding = p; SetSomethingChanged(true);};
	virtual Padding GetPadding() const { return padding; };

	virtual void SetAnchor(Anchor a) { anchor = a; SetSomethingChanged(true);};
	virtual Anchor GetAnchor() const { return anchor; };

	virtual void SetFillType(FillType ft) { fillType = ft; SetSomethingChanged(true);};
	virtual FillType GetFillType() const { return fillType; };

	virtual void SetOffset(const sf::Vector2f& offset) { this->offset = offset; SetSomethingChanged(true);};
	virtual void SetOffset(float x, float y) { offset.x = x; offset.y = y; SetSomethingChanged(true);};

	/*virtual void AddOffset(const sf::Vector2f& offset) { this->offset += offset; SetSomethingChanged(true);};
	virtual void AddOffset(float x, float y) { offset.x += x; offset.y += y; SetSomethingChanged(true);};*/

	virtual void SetScale(const sf::Vector2f& scale) { this->scale = scale; SetSomethingChanged(true);};
	virtual void SetScale(float x, float y) { this->scale.x = x; this->scale.y = y; SetSomethingChanged(true);};

	virtual void SetSomethingChanged(bool changeStatus){somethingChanged = changeStatus;};
	virtual bool GetSomethingChanged() { return somethingChanged; };

	virtual sf::Vector2f GetScaledSize() const { return sf::Vector2f(size.x * GetScale().x, size.y * GetScale().y); };

	virtual sf::Vector2f GetPosition() const { return (parent == nullptr) ? position : parent->GetPosition() + position; };

	virtual sf::Vector2f GetScale() const { return scale; };

	virtual void SetName(std::string n) { this->name = n; };
	virtual std::string GetName() const { return name; };

	virtual bool IsVisible() const { return visible; };
	virtual void Show() { visible = true; };
	virtual void Hide() { visible = false; };

	virtual void SetColour(sf::Color c) { r.setFillColor(c); };

	//typename T is Parent class, typename TC is child class (usually the typename of 'this')
	template <typename T> void SetParent(std::shared_ptr<T> parent)
	{
		static_assert(std::is_base_of<BaseUserInterface, T>::value, "T must derive from BaseUserInterface");
		this->parent = std::dynamic_pointer_cast<T>(parent);
		this->parent->SetSomethingChanged(true);
		SetSomethingChanged(true);
	};
	virtual const std::shared_ptr<BaseUserInterface> GetParent() const {return parent;};

	template <typename T> void AddChild(std::shared_ptr<T> child)
	{
		static_assert(std::is_base_of<BaseUserInterface, T>::value, "T must derive from BaseUserInterface");
		children.push_back(std::dynamic_pointer_cast<T>(child));
		child->SetSomethingChanged(true);
		SetSomethingChanged(true);
	};
	template <typename T> void RemoveChild(std::shared_ptr<T> child)
	{
		static_assert(std::is_base_of<BaseUserInterface, T>::value, "T must derive from BaseUserInterface");

		auto objIterator = children.begin();
		while (objIterator != children.end())
		{
			//auto obj = **objIterator;

			if (objIterator == child)
			{
				objIterator = children.erase(objIterator);
				SetSomethingChanged(true);
				break;
			}
			else
			{
				++objIterator;
			}
		}
	};
	virtual const std::vector<std::shared_ptr<BaseUserInterface>>& GetChildren() const {return children;};

	virtual void Draw(Window& window) override
	{
		if (IsVisible())
		{
			window.Draw(r);
		}
	};
	
	bool ContinueToDraw() const override {return IsVisible();};
	
	SharedContext* context;
	std::string uiType;

private:
	//Filltype is AUTO by default. Sets the size based on the scale of the parents or children.
	virtual void SetupSize()
	{
		switch (GetFillType())
		{
		case(FillType::AUTO):
			//Set to size of contents
			if (!children.empty())
			{
				for (auto& child : children)
				{
					size.x += child->GetScaledSize().x;
					size.y += child->GetScaledSize().y;
				}
			}
			else 
			{
				std::cout << "Filltype AUTO must have at least one child." << GetName() << std::endl;
			}
			break;

		case(FillType::SET):
			//Make sure scale variable is set before this is called.
			if (parent)
			{
				size.x = parent->GetScaledSize().x * scale.x - padding.left - padding.right;
				size.y = parent->GetScaledSize().y * scale.y - padding.top - padding.bottom;
			}
			else
			{
				size.x = context->window->GetViewSpace().width * scale.x;
				size.y = context->window->GetViewSpace().height * scale.y;
			}
			break;
		case(FillType::FILLHORIZONTAL):
			//Fill to size of Parent X, keeping aspect ratio
			if (parent)
			{
				size.x = parent->GetScaledSize().x - padding.left - padding.right;
			}
			else
			{
				size.x = context->window->GetViewSpace().width;
			}
			if (!children.empty())
			{
				float totalChildSizeX = 0;
				for (auto& child : children)
				{
					size.y += child->GetScaledSize().y;
					totalChildSizeX += child->GetScaledSize().x;
				}

				float scaleX = size.x / totalChildSizeX;

				for (auto& child : children)
				{
					child->SetScale(child->GetScale().x * scaleX, child->GetScale().y);
				}
			}
			break;
		case(FillType::FILLVERTICAL):
			//Fill to size of Parent Y, keeping aspect ratio
			if (parent)
			{
				size.y = parent->GetScaledSize().y - padding.top - padding.bottom;
			}
			else 
			{
				size.y = context->window->GetViewSpace().height;
			}
			if (!children.empty())
			{
				float totalChildSizeY = 0;

				for (auto& child : children)
				{
					size.x += child->GetScaledSize().x;
					totalChildSizeY += child->GetScaledSize().y;
				}

				float scaleY = size.y / totalChildSizeY;

				for (auto& child : children)
				{
					child->SetScale(child->GetScale().x, child->GetScale().y * scaleY);
				}
			}
			break;
		case(FillType::FILL):
			//Fill to size of Parent regardless of aspect ratio
			if (parent)
			{
				size.x = parent->GetScaledSize().x - padding.left - padding.right;
				size.y = parent->GetScaledSize().y - padding.top - padding.bottom;
			}
			else
			{
				size.x = context->window->GetViewSpace().width;
				size.y = context->window->GetViewSpace().height;
			}
			if (!children.empty())
			{
				float totalChildSizeX = 0, totalChildSizeY = 0;

				//Get size of all children (width and height)
				for (auto& child : children)
				{
					totalChildSizeX += child->GetScaledSize().x;
					totalChildSizeY += child->GetScaledSize().y;
				}
				//Scaled to fit stretching contents
				float scaleX = size.x / totalChildSizeX, scaleY = size.y / totalChildSizeY;

				for (auto& child : children)
				{
					child->SetScale(child->GetScale().x * scaleX, child->GetScale().y * scaleY);
				}

			}
			break;
		default:
			break;
		}
	};

	virtual void SetPositon()
	{
		float pX, pY, pWidth, pHeight;

		if (GetParent())
		{
			pX = GetParent()->GetPosition().x;
			pY = GetParent()->GetPosition().y;
			pWidth = GetParent()->GetScaledSize().x;
			pHeight = GetParent()->GetScaledSize().y;
		}
		else 
		{
			pX = context->window->GetViewSpace().left;
			pY = context->window->GetViewSpace().top;
			pWidth = context->window->GetViewSpace().width;
			pHeight = context->window->GetViewSpace().height;
		}
		switch (GetAnchor())
		{
		case(Anchor::TOPLEFT):
			position.x = padding.left + offset.x + pX;
			position.y = padding.top + offset.y + pY;
			break;
		case(Anchor::TOPMID):
			position.x = pWidth / 2 + offset.x + pX;
			position.y = padding.top + offset.y + pY;
			break;
		case(Anchor::TOPRIGHT):
			position.x = pWidth - GetScaledSize().x - padding.right + offset.x + pX;
			position.y = padding.top + offset.y + pY;
			break;
		case(Anchor::MIDLEFT):
			position.x = padding.left + offset.x + pX;
			position.y = pHeight / 2 + offset.y + pY;
			break;
		case(Anchor::CENTER):
			position.x = pWidth / 2 + offset.x + pX;
			position.y = pHeight / 2 + offset.y + pY;
			break;
		case(Anchor::MIDRIGHT):
			position.x = pWidth - GetScaledSize().x - padding.right + offset.x + pX;
			position.y = pHeight / 2 + offset.y + pY;
			break;
		case(Anchor::BOTLEFT):
			position.x = padding.left + offset.x + pX;
			position.y = pHeight - GetScaledSize().y - padding.bottom + offset.y + pY;
			break;
		case(Anchor::BOTMID):
			position.x = pWidth / 2 + offset.x + pX;
			position.y = pHeight - GetScaledSize().y - padding.bottom + offset.y + pY;
			break;
		case(Anchor::BOTRIGHT):
			position.x = pWidth - GetScaledSize().x - padding.right + offset.x + pX;
			position.y = pHeight - GetScaledSize().y - padding.bottom + offset.y + pY;
			break;
		default:
			break;
		}

	};

	bool somethingChanged;
	bool visible;
	std::string name;
	std::shared_ptr<BaseUserInterface> parent;
	std::vector<std::shared_ptr<BaseUserInterface>> children;

	Padding padding;
	Anchor anchor;
	FillType fillType;
	sf::Vector2f offset;
	sf::Vector2f position;
	sf::Vector2f size;
	sf::Vector2f scale;

	sf::RectangleShape r;
};

static std::unordered_map<std::string, Anchor> const strToAnchor =
{
	{"TOPLEFT", Anchor::TOPLEFT},
	{"TOPMID", Anchor::TOPMID},
	{"TOPRIGHT", Anchor::TOPRIGHT},
	{"MIDLEFT", Anchor::MIDLEFT},
	{"CENTER", Anchor::CENTER},
	{"MIDRIGHT", Anchor::MIDRIGHT},
	{"BOTLEFT", Anchor::BOTLEFT},
	{"BOTMID", Anchor::BOTMID},
	{"BOTRIGHT", Anchor::BOTRIGHT}
};

static std::unordered_map<std::string, FillType> const strToFillType =
{
	{"AUTO", FillType::AUTO},
	{"SET", FillType::SET},
	{"FILLHORIZONTAL", FillType::FILLHORIZONTAL},
	{"FILLVERTICAL", FillType::FILLVERTICAL},
	{"FILL", FillType::FILL}
};

#endif // BaseUserInterface_h
