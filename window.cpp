#include "window.h"
Window::Window(const std::string& windowName) : window(sf::VideoMode(1920, 1080), windowName, sf::Style::Fullscreen) //RenderWindow constructor
{
    window.setVerticalSyncEnabled(true); // Used to reduce visual artifacts
}
void Window::Update()
{
    sf::Event event; // Event for closing the window
    if (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}
void Window::BeginDraw() // Clears the screen before drawing
{
    window.clear(sf::Color::White);
}
void Window::Draw(const sf::Drawable& drawable) //Draws requested drawable item
{
    window.draw(drawable);
}
void Window::Draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type) //Draws shape based on vertices
{
    window.draw(vertices, vertexCount, type);
}
void Window::EndDraw() //Displays the drawable items we are drawing
{
    window.display();
}
bool Window::IsOpen() const //Checks if window is open
{
    return window.isOpen();
}

sf::Vector2u Window::GetCenter()
{
    sf::Vector2u center;
    center.x = window.getSize().x / 2;
    center.y = window.getSize().y / 2;

    return center;
}
const sf::View& Window::GetView() const
{
    return window.getView();
}
void Window::SetView(const sf::View& view)
{
    window.setView(view);
}

sf::FloatRect Window::GetViewSpace() const
{
    const sf::View& view = GetView();
    const sf::Vector2f& viewCenter = view.getCenter();
    const sf::Vector2f& viewSize = view.getSize();
    sf::Vector2f viewSizeHalf(viewSize.x * 0.5f, viewSize.y * 0.5f);
    sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
    return viewSpace;
}