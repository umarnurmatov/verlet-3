#pragma once
#include "solver.hpp"
#include "SFML/Graphics.hpp"

class Renderer
{
    sf::RenderTarget& m_target;
    float m_outline_thick;
    sf::Color m_fill_col;
    sf::Color m_outl_col; 

    sf::CircleShape m_circle;
public:
    Renderer(sf::RenderTarget& target);

    void render(Solver& solver);
};