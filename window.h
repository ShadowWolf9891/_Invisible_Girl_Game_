#pragma once
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
class Window //Class to wrap RenderWindow and add functionality later.
{
public:
    Window(const std::string& windowName);
    void Update(sf::Time deltaTime);
    
    void BeginDraw();
    void Draw(const sf::Drawable& drawable);
    void Draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type);
    void EndDraw();
    bool IsOpen() const;

    void Close()
    {
        this->window.close();
    };

    sf::Vector2u GetCenter();
    const sf::View& GetView() const;
    void SetView(const sf::View& view);

    sf::FloatRect GetViewSpace() const;

    sf::RenderWindow& GetRenderWindow() { return this->window; } ;

private:
    sf::RenderWindow window;
};