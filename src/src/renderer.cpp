#include "renderer.hpp"

Renderer::Renderer(sf::RenderTarget &target)
    : m_target{target},
      m_outline_thick{1.f},
      m_outl_col{sf::Color::Magenta},
      m_fill_col{sf::Color::White}
{
    m_circle.setPointCount(32);
    m_circle.setOutlineThickness(m_outline_thick);
    m_circle.setOutlineColor(m_outl_col);
}

void Renderer::render(Solver &solver)
{
    auto& objects = solver.getObjects();
    for(size_t i = 0; i < objects.size(); i++)
    {
        auto& obj = objects[i];
        m_circle.setRadius(obj.radius - m_outline_thick);
        m_circle.setPosition(obj.pos_current);
        m_target.draw(m_circle);
    }
}