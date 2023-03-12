#pragma once
#include "solver.hpp"
#include "SFML/Graphics.hpp"

class Renderer
{
    sf::RenderTarget& m_target;
public:
    Renderer(sf::RenderTarget& target);

    void render(Solver& solver);
};