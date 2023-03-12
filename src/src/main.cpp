#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "solver.hpp"
#include "renderer.hpp"
#include <cmath>


int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Verlet", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    bool imgui_sfml_init = ImGui::SFML::Init(window);

    Solver solver;
    Renderer renderer{window};

    solver.setConstraint(sf::Vector2f(window.getSize().x * 0.5f, window.getSize().y * 0.5f), 500.f);

    sf::Clock imgui_sfml_deltaClock;
    sf::Clock solver_clock;
    sf::Clock clock;
    float dt = 1.f;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed || (event.key.code == sf::Keyboard::Q && event.key.control)) 
            {
                window.close();
            }

            if(event.type == event.MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left)
            {
                sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                solver.addRectangle(100.f, 100.f, mouse_pos.x, mouse_pos.y, 30.f, 100.f, false);
                //f = !f;
            }
        }

        // if(clock.getElapsedTime().asSeconds() > dt)
        // {
        //     solver.addObject(sf::Vector2f(900.f, 400.f), 50.f, 10.f);
        //     clock.restart();
        // }

        ImGui::SFML::Update(window, imgui_sfml_deltaClock.restart());

        ImGui::InputFloat("Delta time", &dt, 0.1f);

        solver.update(solver_clock.restart().asSeconds());

        window.clear();
        ImGui::SFML::Render(window);
        renderer.render(solver);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}