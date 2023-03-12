#include "renderer.hpp"

Renderer::Renderer(sf::RenderTarget &target)
    : m_target{target}
{
}

void Renderer::render(Solver &solver)
{
    sf::CircleShape circle;
    circle.setPointCount(64);
    circle.setOutlineThickness(2.f);
    circle.setOutlineColor(sf::Color::Blue);
    const auto& objects = solver.getObjects();
    for(const auto& obj : objects)
    {
        circle.setRadius(obj.radius - 2.f);
        circle.setOrigin(sf::Vector2f(obj.radius - 1.f, obj.radius - 1.f));
        circle.setPosition(obj.pos_current);
        circle.setFillColor(obj.col);
        m_target.draw(circle);
    }
}