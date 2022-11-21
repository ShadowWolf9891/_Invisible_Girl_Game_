#pragma once
#ifndef C_UIWorldLabel_h
#define C_UIWorldLabel_h

#include "Component.h"
#include "C_Drawable.h"

class C_UIWorldLabel : public Component, public C_Drawable
{
public:
    C_UIWorldLabel(Object* owner) : Component(owner)
    {

    };
    virtual ~C_UIWorldLabel() = default;
    
    void Start() override;

    void Draw(Window& window) override;
    bool ContinueToDraw() const override;

    void LateUpdate(float deltaTime) override;

    void SetText(const std::string& text);
    void SetBackgroundSize(const sf::Vector2f& size);

    void SetFontColour(const sf::Color& colour);
    void SetBackgroundColour(const sf::Color& colour);

private:
    sf::Text text;
    sf::RectangleShape background;

};

#endif /* C_UIWorldLabel_h */