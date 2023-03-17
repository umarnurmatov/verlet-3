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
    

    solver.setConstraint(window.getSize().x, window.getSize().y);

    sf::Clock imgui_sfml_deltaClock;
    sf::Clock solver_clock;

    sf::Clock clock;
    float current_fps = 0.f;
    int iterations = 8;
    std::vector<float> fps;


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
                solver.addRectangle(5, 5, 10.f, mouse_pos.x, mouse_pos.y, 100.f, false);
            }
        }



        ImGui::SFML::Update(window, imgui_sfml_deltaClock.restart());

        solver.update(solver_clock.restart().asSeconds());

        ImGui::Text(std::to_string(current_fps).c_str());
        ImGui::PlotLines("FPS", fps.data(), fps.size());
        ImGui::Separator();
        ImGui::Text("Objects count: "); ImGui::SameLine();
        ImGui::Text((std::to_string(solver.getObjects().size())).c_str());
        ImGui::Separator();
        ImGui::SliderInt("Physcis Iterations count", &iterations, 1, 16);
        solver.setSubstepsCount(iterations);
        ImGui::Separator();


        window.clear();
        ImGui::SFML::Render(window);
        renderer.render(solver);
        window.display();

        current_fps = 1.f / clock.restart().asSeconds();

        if(fps.size() < 300)

            fps.push_back(current_fps);
        else
        {
            fps.erase(fps.begin());
            fps.push_back(current_fps);
        }
    }

    ImGui::SFML::Shutdown();

    return 0;
}