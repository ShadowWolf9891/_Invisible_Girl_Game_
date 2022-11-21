#pragma once
#include <SFML/Graphics.hpp>
class Window //Class to wrap RenderWindow and add functionality later.
{
public:
    Window(const std::string& windowName);
    void Update();
    
    void BeginDraw();
    void Draw(const sf::Drawable& drawable);
    void Draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type);
    void EndDraw();
    bool IsOpen() const;

    sf::Vector2u GetCenter();
    const sf::View& GetView() const;
    void SetView(const sf::View& view);

    sf::FloatRect GetViewSpace() const;

private:
    sf::RenderWindow window;
};